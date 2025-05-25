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
#include "core/VoltageSourceComponent.h"

class CircuitSolver
{
    Graph graph;

    std::vector<Edge> mst;                                                  // Minimum spanning tree
    Eigen::MatrixXd B;                                                      // Matrica incidencije fundamentalnih kontura i grana
    Eigen::Matrix<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic> Z;  // Matrica impedansi grana
    Eigen::Matrix<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic> Zk; // Matrica impedansi kontura
    Eigen::VectorXcd Vg;                                                    // Vektor napona grana
    Eigen::VectorXcd Ek;                                                    // Vektor napona kontura
    Eigen::VectorXcd Jk;                                                    // Vektor struja kontura
    Eigen::VectorXcd I;                                                     // Vektor struja grana

public:
    CircuitSolver(cnt::PushBackVector<GridComponent *, 1024> gridComponents) : graph(static_cast<int>(gridComponents.size()))
    {
        for (int i = 0; i < gridComponents.size(); i++)
        {
            graph.addEdge(gridComponents[i]);
        }

        mst = graph.bfsMST(0);

        generateB();
        generateZ();
        calculateZk();
        generateVg();
        calculateEk();
        calculateJk();
        calculateI();
    }

    void generateB()
    {
        B = Eigen::MatrixXd(graph.edgesCount - mst.size(), graph.edgesCount);
        B.setZero();

        int contourIndex = 0;
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

            std::cout << "Kontura: ";
            for (int j = 0; j < kontura.size(); j++)
            {
                auto c = kontura[j].second == -1 ? "-" : "";
                std::cout << c << kontura[j].first << " ";
                B(contourIndex, kontura[j].first) = kontura[j].second;
            }
            std::cout << std::endl;
            contourIndex++;
        }

        std::cout << "generateB: " << std::endl;
        for (int i = 0; i < B.rows(); i++)
        {
            for (int j = 0; j < B.cols(); j++)
            {
                std::cout << B(i, j) << " ";
            }
            std::cout << std::endl;
        }
    }

    void generateZ()
    {
        Z = Eigen::Matrix<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic>(graph.edgesCount, graph.edgesCount);
        Z.setZero();

        for (int i = 0; i < Z.rows(); i++)
        {
            PassiveComponent *passiveComponent = dynamic_cast<PassiveComponent *>(graph.edges[i].gridComponent->getComponent());
            if (passiveComponent == nullptr)
                continue;
            Z(i, i) = passiveComponent->getImpedance();
        }

        std::cout << "generateZ: " << std::endl;
        for (int i = 0; i < Z.rows(); i++)
        {
            for (int j = 0; j < Z.cols(); j++)
            {
                std::cout << Z(i, j) << " ";
            }
            std::cout << std::endl;
        }
    }

    void calculateZk()
    {
        Zk = B * Z * B.transpose();

        std::cout << "generateZk: " << std::endl;
        for (int i = 0; i < Zk.rows(); i++)
        {
            for (int j = 0; j < Zk.cols(); j++)
            {
                std::cout << Zk(i, j) << " ";
            }
            std::cout << std::endl;
        }
    }

    void generateVg()
    {
        Vg = Eigen::VectorXd(graph.edgesCount);
        Vg.setZero();
        for (int i = 0; i < Vg.rows(); i++)
        {
            VoltageSourceComponent *voltageSourceComponent = dynamic_cast<VoltageSourceComponent *>(graph.edges[i].gridComponent->getComponent());
            if (voltageSourceComponent == nullptr)
                continue;
            // TODO: Provjeri dal treba ovo "-" ispred
            Vg(i) = -voltageSourceComponent->getVoltage();
        }
        std::cout << "Vg: " << std::endl;
        for (int i = 0; i < Vg.rows(); i++)
        {
            std::cout << Vg(i) << " ";
        }
        std::cout << std::endl;
    }

    void calculateEk()
    {
        Ek = -B * Vg;
        std::cout << "Ek: " << std::endl;
        for (int i = 0; i < Ek.rows(); i++)
        {
            std::cout << Ek(i) << " ";
        }
        std::cout << std::endl;
    }

    void calculateJk()
    {
        Jk = Zk.inverse() * Ek;
        std::cout << "Jk: " << std::endl;
        for (int i = 0; i < Jk.rows(); i++)
        {
            std::cout << Jk(i) << " ";
        }
        std::cout << std::endl;
    }

    void calculateI()
    {
        I = B.transpose() * Jk;
        std::cout << "I: " << std::endl;
        for (int i = 0; i < I.rows(); i++)
        {
            std::cout << I(i) << " ";
        }
        std::cout << std::endl;
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
