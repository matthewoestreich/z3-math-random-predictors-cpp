#pragma once

#include <z3++.h>

struct NodeVersion {
  int major;
  int minor;
  int patch;
};

class V8Predictor {
public:
  V8Predictor(const NodeVersion &version, const std::vector<double> &sequence);
  double predictNext();

private:
  NodeVersion nodeVersion;
  std::vector<double> sequence;
  uint64_t cState0;
  uint64_t cState1;
  z3::context context;
  z3::solver solver;
  z3::expr sState0;
  z3::expr sState1;

  void xorShift128PlusSymbolic();
  void recoverMantissaAndAddToSolver(double value);
  uint64_t xorShift128PlusConcrete();
  double toDouble(uint64_t value);
};