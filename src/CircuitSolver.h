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
    cnt::PushBackVector<GridComponent *, 1024> gridComponents;
    Graph graph;

    std::vector<Edge> mst;                                                  // Minimum spanning tree
    Eigen::MatrixXd B;                                                      // Matrica incidencije fundamentalnih kontura i grana
    Eigen::Matrix<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic> Z;  // Matrica impedansi grana
    Eigen::Matrix<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic> Zk; // Matrica impedansi kontura
    Eigen::VectorXcd Vg;                                                    // Vektor napona grana
    Eigen::VectorXcd Ek;                                                    // Vektor napona kontura
    Eigen::VectorXcd Jk;                                                    // Vektor struja kontura
    Eigen::VectorXcd I;                                                     // Vektor struja grana
    Eigen::VectorXcd V;                                                     // Vektor napona grana
    std::vector<std::tuple<int, int, CurrentSource *>> currentContours;     // Vektor kontura u kojima se nalazi strujni izvor (contourIndex, edgeIndex, CurrentSource)

private:
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

            CurrentSource *currentSource = dynamic_cast<CurrentSource *>(graph.edges[i].gridComponent->getComponent());
            if (currentSource != nullptr)
            {
                currentContours.push_back(std::tuple<int, int, CurrentSource *>(contourIndex, i, currentSource));
            }

            std::vector<std::pair<int, int>> kontura = graph.findCycle(edges);

            for (int j = 0; j < kontura.size(); j++)
            {
                B(contourIndex, kontura[j].first) = kontura[j].second;
            }
            contourIndex++;
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
    }

    void calculateZk()
    {
        Zk = B * Z * B.transpose();
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
            Vg(i) = -voltageSourceComponent->getVoltage();
        }
    }

    void calculateEk()
    {
        Ek = -B * Vg;
    }

    void calculateJk()
    {
        if (currentContours.size() == 0)
        {
            Jk = Zk.inverse() * Ek;
            return;
        }

        Eigen::MatrixXcd Zuu(Zk.rows() - currentContours.size(), Zk.rows() - currentContours.size());
        Eigen::MatrixXcd Zus(Zk.rows() - currentContours.size(), currentContours.size());
        Eigen::VectorXcd Eu(Zk.rows() - currentContours.size());
        Eigen::VectorXcd Is(currentContours.size());
        Zuu.setZero();
        Zus.setZero();
        Eu.setZero();
        Is.setZero();

        for (int i = 0; i < currentContours.size(); i++)
        {
            Is(i) = std::get<2>(currentContours[i])->current;
        }

        int indexEu = 0;
        for (int i = 0; i < Ek.rows(); i++)
        {
            bool containsI = false;
            for (int k = 0; k < currentContours.size(); k++)
            {
                if (std::get<0>(currentContours[k]) == i)
                {
                    containsI = true;
                    break;
                }
            }
            if (!containsI)
            {
                Eu(indexEu) = Ek(i);
                indexEu++;
            }
        }

        int indexI = 0;
        int indexJuu = 0;
        int indexJus = 0;

        for (int i = 0; i < Zk.rows(); i++)
        {
            bool containsI = false;
            for (int k = 0; k < currentContours.size(); k++)
            {
                if (std::get<0>(currentContours[k]) == i)
                {
                    containsI = true;
                    break;
                }
            }
            if (containsI)
                continue;

            for (int j = 0; j < Zk.rows(); j++)
            {
                bool containsJ = false;

                for (int k = 0; k < currentContours.size(); k++)
                {
                    if (std::get<0>(currentContours[k]) == j)
                    {
                        containsJ = true;
                        break;
                    }
                }
                if (!containsJ)
                {
                    Zuu(indexI, indexJuu) = Zk(i, j);
                    indexJuu++;
                }
                else
                {
                    Zus(indexI, indexJus) = Zk(i, j);
                    indexJus++;
                }
            }
            indexI++;
            indexJuu = 0;
            indexJus = 0;
        }

        Eigen::VectorXcd Ju = Zuu.inverse() * (Eu - Zus * Is);

        std::vector<std::complex<double>> vJk;

        for (int i = 0; i < Ju.size(); i++)
        {
            vJk.push_back(Ju(i));
        }
        for (int i = 0; i < currentContours.size(); i++)
        {
            vJk.insert(vJk.begin() + std::get<0>(currentContours[i]), std::get<2>(currentContours[i])->current);
        }

        Jk = Eigen::VectorXcd(vJk.size());
        for (int i = 0; i < vJk.size(); i++)
        {
            Jk(i) = vJk[i];
        }
    }

    void calculateI()
    {
        I = B.transpose() * Jk;
    }

    void calculateV()
    {
        V = Z * I + Vg;
        // V = - (Z * I + Vg);

        for (int i = 0; i < currentContours.size(); i++)
        {
            std::complex<double> v = 0;
            for (int j = 0; j < V.size(); j++)
            {
                v -= B(std::get<0>(currentContours[i]), j) * V[j];
            }
            V(std::get<1>(currentContours[i])) = v;
        }

        // std::cout << "V: " << std::endl;
        // for (int i = 0; i < V.rows(); i++)
        // {
        //     std::cout << V(i) << " ";
        // }
        // std::cout << std::endl;
    }

public:
    CircuitSolver(cnt::PushBackVector<GridComponent *, 1024> gridComponents) : gridComponents(gridComponents), graph(static_cast<int>(gridComponents.size()))
    {
        for (int i = 0; i < gridComponents.size(); i++)
        {
            graph.addEdge(gridComponents[i]);
        }
    }

    void solve()
    {
        if (gridComponents.isEmpty())
            return;
        mst = graph.bfsMST();
        generateB();
        generateZ();
        calculateZk();
        generateVg();
        calculateEk();
        calculateJk();
        calculateI();
        calculateV();
    }

    void setValues()
    {
        for (int i = 0; i < gridComponents.size(); i++)
        {
            gridComponents[i]->setCurrent(I(i).real());
            gridComponents[i]->setVoltage(V(i).real());
        }
    }
};
