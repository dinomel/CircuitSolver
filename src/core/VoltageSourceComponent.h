//
//  VoltageSourceComponent.h
//  CircuitSolver
//
//  Created by Dino Melunovic on 26. 5. 2025..
//

#pragma once
#include "SourceComponent.h"

class VoltageSourceComponent : public SourceComponent
{
public:
  VoltageSourceComponent(
      const td::String &name,
      const td::String &symbol)
      : SourceComponent(name, symbol) {}

  virtual std::complex<double> getVoltage() = 0;
};
