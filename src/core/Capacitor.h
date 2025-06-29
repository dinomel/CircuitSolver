//
//  Capacitor.h
//  CircuitSolver
//
//  Created by Dino Melunovic on 9. 4. 2025..
//

#pragma once
#include "PassiveComponent.h"
#define _USE_MATH_DEFINES
#include <cmath>

class Capacitor : public PassiveComponent
{
public:
    double capacitance; // [μF]

    Capacitor(double capacitance)
        : PassiveComponent("Capacitor", "C"), capacitance(capacitance) {}

    double capacitanceInFarads() const
    {
        return capacitance / 1000000;
    }

    std::complex<double> getImpedance(double frequency) override
    {
        // Z = 1 / (jωC) = -j / (ωC)
        double omega = frequency == 0 ? 0.0000000001 : 2 * M_PI * frequency;
        return std::complex<double>(0, -1 / (omega * capacitanceInFarads()));
    }
};
