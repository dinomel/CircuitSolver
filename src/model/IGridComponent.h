#pragma once
#include <arch/ArchiveIn.h>
#include <arch/ArchiveOut.h>
#include <gui/Properties.h>
#include <gui/IProperty.h>

extern const int gridSize;

class IGridComponent : public gui::IProperty
{
private:
    static gui::Properties _resistorProperties;
    static gui::Properties _capacitorProperties;
    static gui::Properties _inductorProperties;
    static gui::Properties _wireProperties;
    static gui::Properties _dcVoltageProperties;
    static gui::Properties _acVoltageProperties;
    static gui::Properties _currentProperties;

public:
    enum class Type : unsigned char
    {
        Wire = 0,
        Resistor,
        Capacitor,
        Inductor,
        DCVoltageSource,
        ACVoltageSource,
        CurrentSource,
        Node,
    };
    enum class Tool
    {
        Selector = 0,
        AddWire,
        AddResistor,
        AddCapacitor,
        AddInductor,
        AddDCVoltageSource,
        AddACVoltageSource,
        AddCurrentSource,
    };

    IGridComponent()
    {
    }

    virtual ~IGridComponent()
    {
    }

    // IGridComponent interface
    virtual void draw() const = 0;
    virtual void getBoundingRect(gui::Rect &boundRect) = 0;
    virtual void load(arch::ArchiveIn &ar) = 0;
    virtual void save(arch::ArchiveOut &ar) const = 0;
    virtual Type getType() const = 0;
    virtual bool canBeSelected(const gui::Point &pt) const = 0;
    virtual void init() = 0;
    virtual void translate(const gui::Point &delta) = 0;
    virtual void updateShape() = 0;
    virtual double distanceToPointSquared(const gui::Point &pt) const = 0;
    virtual void snapToGrid() = 0;
    virtual void release() = 0;

    // GridComponentFactory
    static IGridComponent *createResistor(const gui::Point &initPoint);

    static IGridComponent *createCapacitor(const gui::Point &initPoint);

    static IGridComponent *createInductor(const gui::Point &initPoint);

    static IGridComponent *createWire(const gui::Point &initPoint);

    static IGridComponent *createDCVoltage(const gui::Point &initPoint);

    static IGridComponent *createACVoltage(const gui::Point &initPoint);

    static IGridComponent *createCurrent(const gui::Point &initPoint);

    static IGridComponent *createNode(const gui::Point &initPoint);

    static IGridComponent *createGridComponent(const gui::Point &initPoint, Tool tool)
    {
        switch (tool)
        {
        case Tool::AddWire:
            return createWire(initPoint);
        case Tool::AddResistor:
            return createResistor(initPoint);
        case Tool::AddCapacitor:
            return createCapacitor(initPoint);
        case Tool::AddInductor:
            return createInductor(initPoint);
        case Tool::AddDCVoltageSource:
            return createDCVoltage(initPoint);
        case Tool::AddACVoltageSource:
            return createACVoltage(initPoint);
        case Tool::AddCurrentSource:
            return createCurrent(initPoint);
        default:
            return nullptr;
        }
    }

    // Factory tool
    static IGridComponent::Tool currentTool;

    static gui::Point getClosestGridPoint(const gui::Point &pt)
    {
        return {
            round(pt.x / gridSize) * gridSize,
            round(pt.y / gridSize) * gridSize,
        };
    }

    // some limits
    static float maxLineWidth;
    static gui::CoordType selectionDisance2;
    static gui::CoordType refreshOffset;

    // PropertyManager
    static void createProperties(IGridComponent::Type gridComponentType, gui::Properties &properties);

    static gui::Properties *getProperties(IGridComponent::Type gridComponentType);

    static td::String *getAttribsDesc();

    static void initProperties();
};
