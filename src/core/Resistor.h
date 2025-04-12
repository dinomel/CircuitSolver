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
    double impedance;

    Resistor(double resistance, double impedance = 0)
        : PassiveComponent("Resistor", "R", "resistor.png"), resistance(resistance), impedance(impedance) {}
};
