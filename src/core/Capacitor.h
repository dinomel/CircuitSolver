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
};
