//
//  PassiveComponent.h
//  CircuitSolver
//
//  Created by Dino Melunovic on 9. 4. 2025..
//

#pragma once
#include "Component.h"

class PassiveComponent : public Component
{
public:
    PassiveComponent(
        const td::String &name,
        const td::String &symbol)
        : Component(name, symbol) {}

    virtual std::complex<double> getImpedance(double frequency) = 0;
};
