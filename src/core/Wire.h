//
//  Wire.h
//  CircuitSolver
//
//  Created by Dino Melunovic on 9. 4. 2025..
//

#pragma once
#include "Component.h"

class Wire : public Component
{
public:
    Wire(const td::String &id = "")
        : Component("Wire", "W", "", id) {}
};
