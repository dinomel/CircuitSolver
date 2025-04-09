//
//  ACVoltageSource.h
//  CircuitSolver
//
//  Created by Dino Melunovic on 9. 4. 2025..
//

#pragma once
#include "SourceComponent.h"

class ACVoltageSource : public SourceComponent
{
public:
  double maxVoltage;
  double frequency;

  ACVoltageSource(double maxVoltage, double frequency = 50, const td::String &id = "")
      : SourceComponent("AC Voltage Source", "V", "ac_voltage_source.png", id),
        maxVoltage(maxVoltage),
        frequency(frequency) {}
};
