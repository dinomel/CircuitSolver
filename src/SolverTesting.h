//
//  SolverTesting.h
//  CircuitSolver
//
//  Created by Dino Melunovic on 24. 5. 2025..
//

#pragma once
#include <iostream>
#include <vector>
#include "Graph.h"
#include <Eigen/Dense>

class SolverTesting
{
    Graph graph;

    std::vector<std::pair<int, int>> mst; // Minimum spanning tree
    std::vector<std::vector<int>> B;      // Matrica incidencije fundamentalnih kontura i grana

public:
    SolverTesting(cnt::PushBackVector<GridComponent *, 1024> gridComponents) : graph(static_cast<int>(gridComponents.size()))
    {
        for (int i = 0; i < gridComponents.size(); i++)
        {
            graph.addEdge(gridComponents[i]);
        }
        
        std::cout << "nodes count: " << graph.nodesCount << std::endl;

        mst = graph.bfsMST(0);
        generateB();
    }

    void generateB()
    {
        // B.resize(mst.size());
        // for (int i = 0; i < mst.size(); i++)
        // {
        //     B[i].resize(graph.V);
        // }
    }

    void printMST()
    {
        std::cout << "Edges in the Minimum Spanning Tree:" << std::endl;
        for (auto &edge : mst)
        {
            std::cout << edge.first << " - " << edge.second << std::endl;
        }
    }
};
