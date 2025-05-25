//
//  CircuitSolver.h
//  CircuitSolver
//
//  Created by Dino Melunovic on 24. 5. 2025..
//

#pragma once
#include <iostream>
#include <vector>
#include <complex>
#include "graph/Graph.h"
#include <Eigen/Dense>
#include "core/PassiveComponent.h"

class CircuitSolver
{
    Graph graph;

    std::vector<Edge> mst;                            // Minimum spanning tree
    std::vector<std::vector<int>> B;                  // Matrica incidencije fundamentalnih kontura i grana
    std::vector<std::vector<std::complex<double>>> R; // Matrica otpora grana

public:
    CircuitSolver(cnt::PushBackVector<GridComponent *, 1024> gridComponents) : graph(static_cast<int>(gridComponents.size()))
    {
        for (int i = 0; i < gridComponents.size(); i++)
        {
            graph.addEdge(gridComponents[i]);
        }

        mst = graph.bfsMST(0);

        generateB();
        generateR();
    }

    void generateB()
    {
        for (int i = 0; i < graph.edgesCount; i++)
        {
            bool edgeIsInMST = false;
            std::vector<Edge> edges = {graph.edges[i]};
            for (int j = 0; j < mst.size(); j++)
            {
                if (graph.edges[i].index == mst[j].index)
                {
                    edgeIsInMST = true;
                    break;
                }
                edges.push_back(mst[j]);
            }
            if (edgeIsInMST)
                continue;

            std::vector<std::pair<int, int>> kontura = graph.findCycle(edges);

            B.push_back(std::vector<int>(graph.edgesCount));

            std::cout << "Kontura: ";
            for (int j = 0; j < kontura.size(); j++)
            {
                auto c = kontura[j].second == -1 ? "-" : "";
                std::cout << c << kontura[j].first << " ";
                B[B.size() - 1][kontura[j].first] = kontura[j].second;
            }

            std::cout << std::endl;
        }

        for (int i = 0; i < B.size(); i++)
        {
            for (int j = 0; j < B[i].size(); j++)
            {
                std::cout << B[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    void generateR()
    {
        R.resize(graph.edgesCount);
        for (int i = 0; i < R.size(); i++)
        {
            R[i].resize(graph.edgesCount);
            PassiveComponent *passiveComponent = dynamic_cast<PassiveComponent *>(graph.edges[i].gridComponent->getComponent());
            if (passiveComponent == nullptr)
                continue;
            R[i][i] = passiveComponent->getImpedance();
        }

        std::cout << "generateR: " << std::endl;
        for (int i = 0; i < R.size(); i++)
        {
            for (int j = 0; j < R[i].size(); j++)
            {
                std::cout << R[i][j] << " ";
            }
            std::cout << std::endl;
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
