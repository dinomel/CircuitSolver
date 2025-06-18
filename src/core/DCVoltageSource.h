//
//  DCVoltageSource.h
//  CircuitSolver
//
//  Created by Dino Melunovic on 9. 4. 2025..
//

#pragma once
#include "VoltageSourceComponent.h"

class DCVoltageSource : public VoltageSourceComponent
{
public:
    double voltage;

    DCVoltageSource(double voltage)
        : VoltageSourceComponent("DC Voltage Source", "V"), voltage(voltage) {}

    std::complex<double> getVoltage() override
    {
        return std::complex<double>(voltage, 0);
    }
};
