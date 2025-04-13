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
        : PassiveComponent("Resistor", "R", "resistor.png"), resistance(resistance), reactance(reactance) {}
};
