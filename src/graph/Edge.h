//
//  Edge.h
//  CircuitSolver
//
//  Created by Dino Melunovic on 24. 5. 2025..
//

#pragma once
#include "../model/GridComponent.h"

class Edge
{
public:
    int index;
    int startNode;
    int endNode;
    GridComponent *gridComponent;

public:
    Edge(int index, int startNode, int endNode, GridComponent *gridComponent) : index(index), startNode(startNode), endNode(endNode), gridComponent(gridComponent)
    {
    }
};