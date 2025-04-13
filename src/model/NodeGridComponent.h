//
//  NodeGridComponent.h
//  CircuitSolver
//
//  Created by Dino Melunovic on 12. 4. 2025..
//

#pragma once
#include "IGridComponent.h"

class NodeGridComponent : public IGridComponent
{
public:
    gui::Point centerPoint;

public:
    NodeGridComponent(const gui::Point &pt)
        : centerPoint(pt)
    {
    }

    virtual ~NodeGridComponent()
    {
    }

    virtual void draw() const
    {
        _shape.drawWire(td::ColorID::Yellow);
    }

    virtual void getBoundingRect(gui::Rect &boundRect)
    {
        boundRect = gui::Rect(gui::Circle(centerPoint, 1));
    }

    virtual void load(arch::ArchiveIn &ar)
    {
    }

    virtual void save(arch::ArchiveOut &ar) const
    {
    }

    virtual Type getType() const
    {
        return Type::Node;
    }

    virtual bool canBeSelected(const gui::Point &pt) const
    {
        return distanceToPointSquared(pt) < 144;
    }

    virtual void init()
    {
        gui::Circle circle(centerPoint, 1);
        _shape.createCircle(circle);
    }

    virtual void translate(const gui::Point &delta)
    {
        centerPoint.x += delta.x;
        centerPoint.y += delta.y;
        updateShape();
    }

    virtual void updateShape()
    {
        gui::Circle circle(centerPoint, 1);
        _shape.updateCircleNodes(circle);
    }

    virtual double distanceToPointSquared(const gui::Point &pt) const
    {
        return (centerPoint.x - pt.x) * (centerPoint.x - pt.x) + (centerPoint.y - pt.y) * (centerPoint.y - pt.y);
    }

    virtual void snapToGrid()
    {
        centerPoint = getClosestGridPoint(centerPoint);
        updateShape();
    }

    virtual void release()
    {
        delete this;
    }

    virtual void getValues(gui::PropertyValues &propValues) const
    {
    }

    virtual void setValues(gui::PropertyValues &propValues)
    {
    }

    virtual void setValue(td::UINT4 key, gui::PropertyValues &propValues)
    {
    }
};
