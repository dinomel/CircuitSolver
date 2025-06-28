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
    double resistance; // [kOhm]
    double reactance;  // [kOhm]

    Resistor(double resistance, double reactance = 0)
        : PassiveComponent("Resistor", "R"), resistance(resistance), reactance(reactance) {}

    double resistanceInOhms() const
    {
        return resistance * 1000;
    }

    double reactanceInOhms() const
    {
        return reactance * 1000;
    }

    std::complex<double> getImpedance(double frequency) override
    {
        return std::complex<double>(resistanceInOhms(), reactanceInOhms());
    }
};
