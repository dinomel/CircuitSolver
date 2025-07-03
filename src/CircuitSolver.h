//
//  CircuitSolver.h
//  CircuitSolver
//
//  Created by Dino Melunovic on 24. 5. 2025..
//

#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <complex>
#include "graph/Graph.h"
#include "core/PassiveComponent.h"
#include "core/VoltageSourceComponent.h"
#include "core/Resistor.h"
#include "core/Inductor.h"
#include "core/Capacitor.h"
#include "property/DefaultSettings.h"
#include "SimpleMatrix.h"

// global parameters
extern DefaultSettings g_defaultSettings;

class CircuitSolver
{
    cnt::PushBackVector<GridComponent *, 1024> gridComponents;
    Graph graph;

    std::vector<Edge> mst;                                                  // Minimum spanning tree
    SimpleMatrix B;                                                      // Matrica incidencije fundamentalnih kontura i grana
    SimpleMatrix Z;  // Matrica impedansi grana
    SimpleMatrix Zk; // Matrica impedansi kontura
    SimpleMatrix Vg;                                                    // Vektor napona grana
    SimpleMatrix Ek;                                                    // Vektor napona kontura
    SimpleMatrix Jk;                                                    // Vektor struja kontura
    SimpleMatrix I;                                                     // Vektor struja grana
    SimpleMatrix V;                                                     // Vektor napona grana
    std::vector<std::tuple<int, int, CurrentSource *>> currentContours;     // Vektor kontura u kojima se nalazi strujni izvor (contourIndex, edgeIndex, CurrentSource)

private:
    void generateB()
    {
        B.resize(graph.edgesCount - mst.size(), graph.edgesCount);
        B.reset();

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
        Z.resize(graph.edgesCount, graph.edgesCount);
        Z.reset();

        for (int i = 0; i < Z.rows(); i++)
        {
            PassiveComponent *passiveComponent = dynamic_cast<PassiveComponent *>(graph.edges[i].gridComponent->getComponent());
            if (passiveComponent == nullptr)
                continue;
            Z(i, i) = passiveComponent->getImpedance(g_defaultSettings.getFrequency());
        }
    }

    void calculateZk()
    {
        Zk = B * Z * B.transpose();
    }

    void generateVg()
    {
        Vg.resize(graph.edgesCount, 1);
        Vg.reset();
        for (int i = 0; i < Vg.rows(); i++)
        {
            VoltageSourceComponent *voltageSourceComponent = dynamic_cast<VoltageSourceComponent *>(graph.edges[i].gridComponent->getComponent());
            if (voltageSourceComponent == nullptr)
                continue;
            Vg(i, 0) = -voltageSourceComponent->getVoltage();
        }
    }

    void calculateEk()
    {
        Ek = (-1) * B * Vg;
    }

    void calculateJk()
    {
        if (currentContours.size() == 0)
        {
            Jk = Zk.inverse() * Ek;
            return;
        }

        SimpleMatrix Zuu(Zk.rows() - currentContours.size(), Zk.rows() - currentContours.size());
        SimpleMatrix Zus(Zk.rows() - currentContours.size(), currentContours.size());
        SimpleMatrix Eu(Zk.rows() - currentContours.size(), 1);
        SimpleMatrix Is(currentContours.size(), 1);
        Zuu.reset();
        Zus.reset();
        Eu.reset();
        Is.reset();

        for (int i = 0; i < currentContours.size(); i++)
        {
            Is(i, 0) = std::get<2>(currentContours[i])->currentInAmpers();
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
                Eu(indexEu, 0) = Ek(i, 0);
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

        SimpleMatrix Ju = Zuu.inverse() * (Eu - Zus * Is);

        std::vector<std::complex<double>> vJk;

        for (int i = 0; i < Ju.rows(); i++)
        {
            vJk.push_back(Ju(i, 0));
        }
        for (int i = 0; i < currentContours.size(); i++)
        {
            vJk.insert(vJk.begin() + std::get<0>(currentContours[i]), std::get<2>(currentContours[i])->currentInAmpers());
        }

        Jk = SimpleMatrix(vJk.size(), 1);
        for (int i = 0; i < vJk.size(); i++)
        {
            Jk(i, 0) = vJk[i];
        }
    }

    void calculateI()
    {
        I = B.transpose() * Jk;
    }

    void calculateV()
    {
        V = Z * I + Vg;

        for (int i = 0; i < currentContours.size(); i++)
        {
            std::complex<double> v = 0;
            for (int j = 0; j < V.rows(); j++)
            {
                v -= B(std::get<0>(currentContours[i]), j) * V(j, 0);
            }
            V(std::get<1>(currentContours[i]), 0) = v;
        }
    }

public:
    CircuitSolver(cnt::PushBackVector<GridComponent *, 1024> gridComponents) : gridComponents(gridComponents), graph(static_cast<int>(gridComponents.size()))
    {
        for (int i = 0; i < gridComponents.size(); i++)
        {
            graph.addEdge(gridComponents[i]);
        }
    }

    std::vector<std::pair<std::complex<double>, std::complex<double>>> solve()
    {
        if (gridComponents.isEmpty())
            return {};
        mst = graph.bfsMST();
        generateB();
        generateZ();
        calculateZk();
        generateVg();
        calculateEk();
        calculateJk();
        calculateI();
        calculateV();
        std::vector<std::pair<std::complex<double>, std::complex<double>>> results = {};
        for (int i = 0; i < gridComponents.size(); i++)
        {
            results.push_back(std::pair(I(i, 0), V(i, 0)));
        }
        return results;
    }

    std::string exportModel() const
    {
        std::string model = "Header:\n\tmaxIter=50\n\treport=AllDetails //Solved - only final solved solution, All - shows solved and nonSolved with iterations, AllWithDetails - All + debug information\nend  //end of header\n";
        model += "// Model generated by CircuitSolver\n\n";
        model += "Model [type = NL, name = \"Circuit Model\", domain=cmplx]:\n";
        model += "\tVars [conj=false, out=true]:\n";
        for (int i = 0; i < graph.edgesCount; i++)
        {
            model += "\t\tI" + std::to_string(i + 1) + "\n";
            model += "\t\tV" + std::to_string(i + 1) + "\n";
            switch (graph.edges[i].gridComponent->getType())
            {
            case IGridComponent::Type::Capacitor:
                model += "\t\tZc" + std::to_string(i + 1) + "\n";
                break;
            case IGridComponent::Type::Inductor:
                model += "\t\tZl" + std::to_string(i + 1) + "\n";
                break;
            default:
                break;
            }
        }
        model += "\tParams:\n";
        model += "\t\tf = " + std::to_string(g_defaultSettings.getFrequency()) + "\n";
        model += "\t\tomega = 2 * pi * f\n";

        std::vector<int> capacitorIndexes = {};
        std::vector<int> inductorIndexes = {};
        std::vector<int> currentSourceIndexes = {};

        for (int i = 0; i < graph.edgesCount; i++)
        {
            VoltageSourceComponent *voltageSource = dynamic_cast<VoltageSourceComponent *>(graph.edges[i].gridComponent->getComponent());
            if (voltageSource != nullptr)
            {
                model += "\t\tVg" + std::to_string(i + 1) + " = " + std::to_string(voltageSource->getVoltage().real()) + " + 1i * " + std::to_string(voltageSource->getVoltage().imag()) + "\n";
                continue;
            }
            CurrentSource *currentSource = dynamic_cast<CurrentSource *>(graph.edges[i].gridComponent->getComponent());
            if (currentSource != nullptr)
            {
                currentSourceIndexes.push_back(i);
                model += "\t\tIg" + std::to_string(i + 1) + " = " + std::to_string(currentSource->currentInAmpers()) + "\n";
                continue;
            }
            Resistor *resistor = dynamic_cast<Resistor *>(graph.edges[i].gridComponent->getComponent());
            if (resistor != nullptr)
            {
                model += "\t\tR" + std::to_string(i + 1) + " = " + std::to_string(resistor->resistanceInOhms()) + " + 1i * " + std::to_string(resistor->reactanceInOhms()) + "\n";
                continue;
            }
            Inductor *inductor = dynamic_cast<Inductor *>(graph.edges[i].gridComponent->getComponent());
            if (inductor != nullptr)
            {
                inductorIndexes.push_back(i);
                model += "\t\tL" + std::to_string(i + 1) + " = " + std::to_string(inductor->inductance) + "\n";
                continue;
            }
            Capacitor *capacitor = dynamic_cast<Capacitor *>(graph.edges[i].gridComponent->getComponent());
            if (capacitor != nullptr)
            {
                capacitorIndexes.push_back(i);
                model += "\t\tC" + std::to_string(i + 1) + " = " + std::to_string(capacitor->capacitanceInFarads()) + "\n";
                continue;
            }
        }

        model += "\tNLEs:\n";

        for (int i = 0; i < capacitorIndexes.size(); i++)
        {
            model += "\t\tZc" + std::to_string(capacitorIndexes[i] + 1) + " = 1 / (1i * omega * C" + std::to_string(capacitorIndexes[i] + 1) + ")\n";
        }
        for (int i = 0; i < inductorIndexes.size(); i++)
        {
            model += "\t\tZl" + std::to_string(inductorIndexes[i] + 1) + " = 1i * omega * L" + std::to_string(inductorIndexes[i] + 1) + "\n";
        }
        for (int i = 0; i < currentSourceIndexes.size(); i++)
        {
            model += "\t\tI" + std::to_string(currentSourceIndexes[i] + 1) + " = Ig" + std::to_string(currentSourceIndexes[i] + 1) + "\n";
        }

        if (!capacitorIndexes.empty() || !inductorIndexes.empty() || !currentSourceIndexes.empty())
            model += "\n";

        model += "\t\t// KZS\n";

        for (int i = 0; i < graph.nodesCount - 1; i++)
        {
            model += "\t\t";
            bool isFirst = true;
            for (int j = 0; j < graph.edgesCount; j++)
            {
                std::string edgeName = "I" + std::to_string(j + 1);
                if (graph.edges[j].endNode == i)
                {
                    model += isFirst ? edgeName : " + " + edgeName;
                    isFirst = false;
                }
                else if (graph.edges[j].startNode == i)
                {
                    model += (isFirst ? "- " : " - ") + edgeName;
                    isFirst = false;
                }
            }
            model += " = 0\n";
        }

        model += "\n\t\t// KZN\n";

        for (int i = 0; i < B.rows(); i++)
        {
            bool hasCurrentSource = false;
            for (int j = 0; j < currentContours.size(); j++)
            {
                if (std::get<0>(currentContours[j]) == i)
                {
                    hasCurrentSource = true;
                    break;
                }
            }
            if (hasCurrentSource)
                continue;

            model += "\t\t";
            bool isFirst = true;
            for (int j = 0; j < B.cols(); j++)
            {
                if (B(i, j).real() == 0)
                    continue;
                switch (graph.edges[j].gridComponent->getType())
                {
                case IGridComponent::Type::Resistor:
                    model += B(i, j).real() < 0 ? (isFirst ? "" : " + ") : (isFirst ? "- " : " - ");
                    model += "R" + std::to_string(j + 1) + " * " + "I" + std::to_string(j + 1);
                    isFirst = false;
                    break;
                case IGridComponent::Type::Capacitor:
                    model += B(i, j).real() < 0 ? (isFirst ? "" : " + ") : (isFirst ? "- " : " - ");
                    model += "Zc" + std::to_string(j + 1) + " * " + "I" + std::to_string(j + 1);
                    isFirst = false;
                    break;
                case IGridComponent::Type::Inductor:
                    model += B(i, j).real() < 0 ? (isFirst ? "" : " + ") : (isFirst ? "- " : " - ");
                    model += "Zl" + std::to_string(j + 1) + " * " + "I" + std::to_string(j + 1);
                    isFirst = false;
                    break;
                case IGridComponent::Type::DCVoltageSource:
                case IGridComponent::Type::ACVoltageSource:
                    model += B(i, j).real() > 0 ? (isFirst ? "" : " + ") : (isFirst ? "- " : " - ");
                    model += "Vg" + std::to_string(j + 1);
                    isFirst = false;
                    break;
                default:
                    break;
                }
            }
            model += " = 0\n";
        }

        model += "\n\t\t// Naponi grana\n";

        for (int i = 0; i < graph.edgesCount; i++)
        {
            switch (graph.edges[i].gridComponent->getType())
            {
            case IGridComponent::Type::Resistor:
                model += "\t\tV" + std::to_string(i + 1) + " = R" + std::to_string(i + 1) + " * " + "I" + std::to_string(i + 1) + "\n";
                break;
            case IGridComponent::Type::Capacitor:
                model += "\t\tV" + std::to_string(i + 1) + " = Zc" + std::to_string(i + 1) + " * " + "I" + std::to_string(i + 1) + "\n";
                break;
            case IGridComponent::Type::Inductor:
                model += "\t\tV" + std::to_string(i + 1) + " = Zl" + std::to_string(i + 1) + " * " + "I" + std::to_string(i + 1) + "\n";
                break;
            case IGridComponent::Type::DCVoltageSource:
            case IGridComponent::Type::ACVoltageSource:
                model += "\t\tV" + std::to_string(i + 1) + " = - Vg" + std::to_string(i + 1) + "\n";
                break;
            case IGridComponent::Type::Wire:
                model += "\t\tV" + std::to_string(i + 1) + " = 0\n";
                break;
            default:
                break;
            }
        }

        for (int i = 0; i < currentContours.size(); i++)
        {
            model += "\t\tV" + std::to_string(std::get<1>(currentContours[i]) + 1) + " = ";
            bool isFirst = true;

            for (int j = 0; j < graph.edgesCount; j++)
            {
                if (j == std::get<1>(currentContours[i]))
                    continue;

                double b = B(std::get<0>(currentContours[i]), j).real();
                if (b == 0)
                    continue;

                model += (b < 0 ? (isFirst ? "V" : " + V") : (isFirst ? "- V" : " - V")) + std::to_string(j + 1);
                isFirst = false;
            }
            model += "\n";
        }

        model += "end";

        std::cout << model << std::endl;
        return model;
    }
};
