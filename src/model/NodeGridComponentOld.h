//
//  NodeGridComponent.h
//  CircuitSolver
//
//  Created by Dino Melunovic on 11. 4. 2025..
//

#pragma once
#include "GridComponent.h"

class NodeGridComponentOld : public GridComponent
{
public:
    int parentComponentID;
    bool isStartNode;

public:
    NodeGridComponentOld(NodeGridComponent *startNode, NodeGridComponent *endNode, int parentComponentID, bool isStartNode)
        : GridComponent(startNode, endNode, 0, 0), parentComponentID(parentComponentID), isStartNode(isStartNode)
    {
    }

    void updateNodePoint(const gui::Point &point)
    {
        startNode->centerPoint = point;
        endNode->centerPoint = point;
        updateShape();
    }

    virtual Component *getComponent()
    {
        return nullptr;
    }

    virtual Type getType() const
    {
        return Type::Resistor;
    }

    virtual void load(arch::ArchiveIn &ar)
    {
        //        GridComponent::load(ar);
        //        ar >> _startPoint;
    }

    virtual void save(arch::ArchiveOut &ar) const
    {
        //        GridComponent::save(ar);
        //        ar << _startPoint;
    }

    virtual void getValues(gui::PropertyValues &propValues) const
    {
    }

    virtual void setValues(gui::PropertyValues &propValues)
    {
    }

    void init()
    {
        gui::Circle circle(getStartPoint(), 1);
        _shape.createCircle(circle);
    }

    virtual void updateShape()
    {

        gui::Circle circle(getStartPoint(), 1);
        _shape.updateCircleNodes(circle);
    }

    virtual double distanceToPointSquared(const gui::Point &pt) const
    {
        return (getStartPoint().x - pt.x) * (getStartPoint().x - pt.x) + (getStartPoint().y - pt.y) * (getStartPoint().y - pt.y);
    }
};
