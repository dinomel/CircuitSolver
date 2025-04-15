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

    Component *getComponent() override
    {
        return &_wire;
    }

    Type getType() const override
    {
        return Type::Wire;
    }

    void init() override
    {
        gui::Point points[] = {
            getStartPoint(),
            getEndPoint(),
        };
        _shape.createLines(&points[0], 2);
    }

    void updateShape() override
    {
        init();
    }
};
