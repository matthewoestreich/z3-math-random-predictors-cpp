#pragma once

#include <z3++.h>

class FirefoxPredictor {
private:
  std::vector<double> sequence;
  uint64_t cState0;
  uint64_t cState1;
  z3::context context;
  z3::solver solver;
  z3::expr sState0;
  z3::expr sState1;
  z3::expr kDoubleSignificantMaskSymbolic;
  static constexpr int kDoubleSignificantBits = 53;
  static constexpr uint64_t kDoubleSignificantMask = 0x1FFFFFFFFFFFFF;

  void xorShift128PlusSymbolic();
  uint64_t xorShift128PlusConcrete();
  uint64_t recoverMantissa(double value);
  double toDouble(uint64_t value);

public:
  FirefoxPredictor(const std::vector<double> &sequence);
  double predictNext();
};
