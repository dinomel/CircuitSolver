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
        const td::String &image,
        const td::String &id = "")
        : Component(name, symbol, image, id) {}
};
