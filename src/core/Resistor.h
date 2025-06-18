//
//  Resistor.h
//  CircuitSolver
//
//  Created by Dino Melunovic on 9. 4. 2025..
//

#pragma once
#include "PassiveComponent.h"

class Resistor : public PassiveComponent
{
public:
    double resistance;
    double reactance;

    Resistor(double resistance, double reactance = 0)
        : PassiveComponent("Resistor", "R"), resistance(resistance), reactance(reactance) {}

    std::complex<double> getImpedance(double frequency) override
    {
        return std::complex<double>(resistance, reactance);
    }
};
