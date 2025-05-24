//
//  CircuitSolver.h
//  CircuitSolver
//
//  Created by Dino Melunovic on 24. 5. 2025..
//

#pragma once
#include <iostream>
#include <vector>
#include "graph/Graph.h"
#include <Eigen/Dense>

class CircuitSolver
{
    Graph graph;

    std::vector<Edge> mst; // Minimum spanning tree
    std::vector<std::vector<int>> B;      // Matrica incidencije fundamentalnih kontura i grana

public:
    CircuitSolver(cnt::PushBackVector<GridComponent *, 1024> gridComponents) : graph(static_cast<int>(gridComponents.size()))
    {
        for (int i = 0; i < gridComponents.size(); i++)
        {
            graph.addEdge(gridComponents[i]);
        }

        mst = graph.bfsMST(0);
        generateB();
    }

    void generateB()
    {
        B.resize(mst.size());
        for (int i = 0; i < mst.size(); i++)
        {
            B[i].resize(graph.edgesCount);
            std::pair<int, int> mst[i];
            for (int j = 0; j < graph.edgesCount; j++)
            {
                graph.adj[j];
                graph.adjC[i][j] == nullptr;
            }
        }
    }

    void printMST()
    {
        std::cout << "Edges in the Minimum Spanning Tree:" << std::endl;
        for (auto &edge : mst)
        {
            std::cout << edge.startNode << " - " << edge.endNode << std::endl;
        }
    }
};
