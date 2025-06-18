//
//  Inductor.h
//  CircuitSolver
//
//  Created by Dino Melunovic on 9. 4. 2025..
//

#pragma once
#include "PassiveComponent.h"

class Inductor : public PassiveComponent
{
public:
    double inductance;

    Inductor(double inductance)
        : PassiveComponent("Inductor", "L"), inductance(inductance) {}

    std::complex<double> getImpedance(double frequency) override
    {
        // Z = jωL
        double omega = 2 * M_PI * frequency;
        return std::complex<double>(0, omega * inductance);
    }
};
