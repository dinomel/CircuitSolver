//
//  CurrentSource.h
//  CircuitSolver
//
//  Created by Dino Melunovic on 17. 4. 2025..
//

#pragma once
#include "SourceComponent.h"

class CurrentSource : public SourceComponent
{
public:
    double current; // [mA]

public:
    CurrentSource(double current)
        : SourceComponent("Current Source", "I"), current(current) {}

    double currentInAmpers() const
    {
        return current / 1000;
    }
};
