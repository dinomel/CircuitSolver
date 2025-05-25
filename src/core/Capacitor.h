//
//  Capacitor.h
//  CircuitSolver
//
//  Created by Dino Melunovic on 9. 4. 2025..
//

#pragma once
#include "PassiveComponent.h"

class Capacitor : public PassiveComponent
{
public:
    double capacitance;

    Capacitor(double capacitance)
        : PassiveComponent("Capacitor", "C", "capacitor.png"), capacitance(capacitance) {}

    std::complex<double> getImpedance() override
    {
        // TODO: Provjeri ovo
        return std::complex<double>(0, -1 / (3.14 * 50 * capacitance));
    }
};
