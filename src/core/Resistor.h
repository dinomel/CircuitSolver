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

    Resistor(double resistance, const td::String &id = "")
        : PassiveComponent("Resistor", "R", "resistor.png", id), resistance(resistance) {}
};
