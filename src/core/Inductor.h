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

    Inductor(double inductance, const td::String &id = "")
        : PassiveComponent("Inductor", "L", "inductor.png", id), inductance(inductance) {}
};
