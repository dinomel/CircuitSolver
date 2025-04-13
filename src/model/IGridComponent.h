#pragma once
#include <gui/Shape.h>
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

protected:
    gui::Shape _shape;

public:
    enum class Type : unsigned char
    {
        Wire = 0,
        Resistor,
        Capacitor,
        Inductor,
        Node,
    };
    enum class Tool
    {
        Selector = 0,
        AddWire,
        AddResistor,
        AddCapacitor,
        AddInductor
    };

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

    static IGridComponent *createNode(const gui::Point &initPoint);

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
    static void createProperties(IGridComponent::Type shapeType, gui::Properties &properties);

    static gui::Properties *getProperties(IGridComponent::Type shapeType);

    static td::String *getAttribsDesc();

    static void initProperties();
};
