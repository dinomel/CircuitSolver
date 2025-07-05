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
    static gui::Properties _groundProperties;

public:
    enum class Type : unsigned char
    {
        Wire,
        Resistor,
        Capacitor,
        Inductor,
        DCVoltageSource,
        ACVoltageSource,
        CurrentSource,
        Ground,
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
        AddGround,
    };

    IGridComponent()
    {
    }

    virtual ~IGridComponent()
    {
    }

    // IGridComponent interface
    virtual void draw(bool isSelected) const = 0;
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
    static IGridComponent *createResistor(const gui::Point &startPoint, const gui::Point &endPoint);

    static IGridComponent *createCapacitor(const gui::Point &startPoint, const gui::Point &endPoint);

    static IGridComponent *createInductor(const gui::Point &startPoint, const gui::Point &endPoint);

    static IGridComponent *createWire(const gui::Point &startPoint, const gui::Point &endPoint);

    static IGridComponent *createDCVoltage(const gui::Point &startPoint, const gui::Point &endPoint);

    static IGridComponent *createACVoltage(const gui::Point &startPoint, const gui::Point &endPoint);

    static IGridComponent *createCurrent(const gui::Point &startPoint, const gui::Point &endPoint);

    static IGridComponent *createNode(const gui::Point &initPoint);

    static IGridComponent *createGround(const gui::Point &startPoint, const gui::Point &endPoint);

    static Tool typeToTool(Type type)
    {
        switch (type)
        {
        case Type::Wire:
            return Tool::AddWire;
        case Type::Resistor:
            return Tool::AddResistor;
        case Type::Capacitor:
            return Tool::AddCapacitor;
        case Type::Inductor:
            return Tool::AddInductor;
        case Type::DCVoltageSource:
            return Tool::AddDCVoltageSource;
        case Type::ACVoltageSource:
            return Tool::AddACVoltageSource;
        case Type::CurrentSource:
            return Tool::AddCurrentSource;
        case Type::Ground:
            return Tool::AddGround;
        default:
            return Tool::Selector;
        }
    }

    static IGridComponent *createGridComponent(const gui::Point &startPoint, const gui::Point &endPoint, Type type)
    {
        return createGridComponent(startPoint, endPoint, typeToTool(type));
    }

    static IGridComponent *createGridComponent(const gui::Point &startPoint, const gui::Point &endPoint, Tool tool)
    {
        switch (tool)
        {
        case Tool::AddWire:
            return createWire(startPoint, endPoint);
        case Tool::AddResistor:
            return createResistor(startPoint, endPoint);
        case Tool::AddCapacitor:
            return createCapacitor(startPoint, endPoint);
        case Tool::AddInductor:
            return createInductor(startPoint, endPoint);
        case Tool::AddDCVoltageSource:
            return createDCVoltage(startPoint, endPoint);
        case Tool::AddACVoltageSource:
            return createACVoltage(startPoint, endPoint);
        case Tool::AddCurrentSource:
            return createCurrent(startPoint, endPoint);
        case Tool::AddGround:
            return createGround(startPoint, endPoint);
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
    static float maxFrequency;

    // PropertyManager
    static gui::Properties *getProperties(IGridComponent::Type gridComponentType);

    static void initProperties();
};
