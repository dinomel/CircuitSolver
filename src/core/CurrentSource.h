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
    double current;

    CurrentSource(double current)
        : SourceComponent("Current Source", "I", "dc_voltage_source.png"), current(current) {}
};
