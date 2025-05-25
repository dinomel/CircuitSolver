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

    std::vector<Edge> mst;           // Minimum spanning tree
    std::vector<std::vector<int>> B; // Matrica incidencije fundamentalnih kontura i grana

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

            // std::vector<int> konturaTest = graph.findCycle();
            // std::cout << "konturaTest: ";
            // for (int j = 0; j < konturaTest.size(); j++)
            // {
            //     std::cout << konturaTest[j] << " ";
            // }
            // std::cout << std::endl;

            // Dovde u kodu dodje samo ako je graph.edges[i] viseca grana, odnosno nije u MST

            // Vraca vektor edge indexa koji mogu biti negativni i ukoliko jeste, to znaci da je B[.][i] = -1
            std::vector<int> kontura = graph.findCycle(edges);

            B.push_back(std::vector<int>(graph.edgesCount));

            std::cout << "Kontura: ";
            for (int j = 0; j < kontura.size(); j++)
            {
                std::cout << kontura[j] << " ";

                if (kontura[j] < 0)
                {
                    B[B.size() - 1][-kontura[j]] = -1;
                }
                else
                {
                    B[B.size() - 1][kontura[j]] = 1;
                }

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

    void printMST()
    {
        std::cout << "Edges in the Minimum Spanning Tree:" << std::endl;
        for (auto &edge : mst)
        {
            std::cout << edge.startNode << " - " << edge.endNode << std::endl;
        }
    }
};
