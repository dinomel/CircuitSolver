//
//  ACVoltageSource.h
//  CircuitSolver
//
//  Created by Dino Melunovic on 9. 4. 2025..
//

#pragma once
#include "VoltageSourceComponent.h"

class ACVoltageSource : public VoltageSourceComponent
{
public:
  double maxVoltage;
  double frequency;
  double phaseAngle; // Radians

  ACVoltageSource(double maxVoltage, double frequency = 50, double phaseAngle = 0)
      : VoltageSourceComponent("AC Voltage Source", "V", "ac_voltage_source.png"),
        maxVoltage(maxVoltage),
        frequency(frequency),
        phaseAngle(phaseAngle) {}

  std::complex<double> getVoltage() override
  {
    double Vrms = maxVoltage / sqrt(2);
    return std::complex<double>(Vrms * cos(phaseAngle), Vrms * sin(phaseAngle));
  }
};
