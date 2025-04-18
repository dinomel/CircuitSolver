//
//  DCVoltageSource.h
//  CircuitSolver
//
//  Created by Dino Melunovic on 9. 4. 2025..
//

#pragma once
#include "SourceComponent.h"

class DCVoltageSource : public SourceComponent
{
public:
    double voltage;

    DCVoltageSource(double voltage)
        : SourceComponent("DC Voltage Source", "V", "dc_voltage_source.png"), voltage(voltage) {}
};
