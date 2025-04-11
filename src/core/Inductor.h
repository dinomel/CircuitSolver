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
        : PassiveComponent("Inductor", "L", "inductor.png"), inductance(inductance) {}
};
