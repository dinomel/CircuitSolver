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
        const td::String &symbol,
        const td::String &image)
        : Component(name, symbol, image) {}

    virtual std::complex<double> getImpedance() = 0;
};
