//
//  WireGridComponent.h
//  CircuitSolver
//
//  Created by Dino Melunovic on 13. 4. 2025..
//

#pragma once
#include "GridComponent.h"
#include "../core/Wire.h"
#include "../core/Component.h"

class WireGridComponent : public GridComponent
{
protected:
    Wire _wire;

public:
    WireGridComponent(NodeGridComponent *startNode, NodeGridComponent *endNode)
        : GridComponent(startNode, endNode, 0, 0)
    {
    }

    virtual Component *getComponent()
    {
        return &_wire;
    }

    virtual Type getType() const
    {
        return Type::Wire;
    }

    virtual void init()
    {
        gui::Point points[] = {
            getStartPoint(),
            getEndPoint(),
        };
        _shape.createLines(&points[0], 2);
    }

    virtual void updateShape()
    {
        gui::Point points[] = {
            getStartPoint(),
            getEndPoint(),
        };
        _shape.createLines(&points[0], 2);
    }

    virtual void initProperties(gui::Properties *properties) const
    {
        initNodesProperties(properties);
    }

    virtual void getValues(gui::PropertyValues &propValues) const
    {
        getNodesValues(propValues);
    }

    virtual void setValues(gui::PropertyValues &propValues)
    {
        setNodesValues(propValues);

        propValues.setUpdateCanvas(true);
        startNode->updateShape();
        endNode->updateShape();
        updateShape();
    }
};
