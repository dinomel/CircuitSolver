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

    Resistor(double resistance)
        : PassiveComponent("Resistor", "R", "resistor.png"), resistance(resistance) {}
};
