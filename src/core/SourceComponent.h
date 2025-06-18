//
//  SourceComponent.h
//  CircuitSolver
//
//  Created by Dino Melunovic on 9. 4. 2025..
//

#pragma once
#include "Component.h"

class SourceComponent : public Component
{
public:
    SourceComponent(
        const td::String &name,
        const td::String &symbol)
        : Component(name, symbol) {}
};
