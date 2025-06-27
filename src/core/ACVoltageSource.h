//
//  ACVoltageSource.h
//  CircuitSolver
//
//  Created by Dino Melunovic on 9. 4. 2025..
//

#pragma once
#include "VoltageSourceComponent.h"
#define _USE_MATH_DEFINES
#include <cmath>

class ACVoltageSource : public VoltageSourceComponent
{
public:
  double maxVoltage;
  double phaseAngle; // Degrees

  ACVoltageSource(double maxVoltage, double phaseAngle = 0)
      : VoltageSourceComponent("AC Voltage Source", "V"),
        maxVoltage(maxVoltage),
        phaseAngle(phaseAngle) {}

  std::complex<double> getVoltage() override
  {
    return std::polar(maxVoltage / sqrt(2), phaseAngle / 180 * M_PI);
  }
};
