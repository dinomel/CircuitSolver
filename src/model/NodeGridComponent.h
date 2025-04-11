//
//  NodeGridComponent.h
//  CircuitSolver
//
//  Created by Dino Melunovic on 11. 4. 2025..
//

#pragma once
#include "GridComponent.h"

class NodeGridComponent : public GridComponent
{
public:
    int parentComponentID;

public:
    NodeGridComponent(const gui::Point &point, int parentComponentID, td::ColorID fillColor, td::ColorID lineColor)
        : GridComponent(point, 0, 0, fillColor, lineColor), parentComponentID(parentComponentID)
    {
    }

    void updateNodePoint(const gui::Point &point)
    {
        _startPoint = point;
        _endPoint = point;
        updateLineNodes();
    }

    virtual Component *getComponent()
    {
        return nullptr;
    }

    virtual Type getType() const
    {
        return Type::Resistor;
    }

    virtual void getBoundingRect(gui::Rect &boundRect)
    {
        boundRect = gui::Rect(_startPoint.x, _startPoint.y, _endPoint.x, _endPoint.y);
    }

    virtual void load(arch::ArchiveIn &ar)
    {
        GridComponent::load(ar);
        ar >> _startPoint;
    }

    virtual void save(arch::ArchiveOut &ar) const
    {
        GridComponent::save(ar);
        ar << _startPoint;
    }

    virtual void getValues(gui::PropertyValues &propValues) const
    {
    }

    virtual void setValues(gui::PropertyValues &propValues)
    {
    }

    void init()
    {
        gui::Circle circle(_startPoint, 1);
        _shape.createCircle(circle);
    }

    virtual void updateLineNodes()
    {

        gui::Circle circle(_startPoint, 1);
        _shape.updateCircleNodes(circle);
    }
};
