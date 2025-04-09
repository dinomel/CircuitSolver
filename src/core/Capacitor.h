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

    Capacitor(double capacitance, const td::String &id = "")
        : PassiveComponent("Capacitor", "C", "capacitor.png", id), capacitance(capacitance) {}
};
