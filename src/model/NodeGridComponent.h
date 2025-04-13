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
protected:
    gui::Point _centerPoint;

public:
    NodeGridComponent(const gui::Point &pt)
        : _centerPoint(pt)
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
        boundRect = gui::Rect(gui::Circle(_centerPoint, 1));
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
        gui::Circle circle(_centerPoint, 1);
        _shape.createCircle(circle);
    }

    virtual void translate(const gui::Point &delta)
    {
        _centerPoint.x += delta.x;
        _centerPoint.y += delta.y;
        updateShape();
    }

    virtual void updateShape()
    {
        gui::Circle circle(_centerPoint, 1);
        _shape.updateCircleNodes(circle);
    }

    virtual double distanceToPointSquared(const gui::Point &pt) const
    {
        return (_centerPoint.x - pt.x) * (_centerPoint.x - pt.x) + (_centerPoint.y - pt.y) * (_centerPoint.y - pt.y);
    }

    virtual void snapToGrid()
    {
        _centerPoint = getClosestGridPoint(_centerPoint);
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
