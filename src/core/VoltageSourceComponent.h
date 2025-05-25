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
      const td::String &symbol,
      const td::String &image = "")
      : SourceComponent(name, symbol, image) {}

  virtual std::complex<double> getVoltage() = 0;
};
