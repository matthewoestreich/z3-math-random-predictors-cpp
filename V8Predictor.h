#pragma once

#include <cstdint>
#include <vector>
#include <z3++.h>

using namespace std;
using namespace z3;

class V8Predictor {
public:
  explicit V8Predictor(vector<double> sequence);
  double predictNext();

private:
  const uint64_t mask = 0xFFFFFFFFFFFFFFFF;
  vector<double> sequence;
  uint64_t cState0 = 0;
  uint64_t cState1 = 0;
  context context;
  solver solver;
  expr sState0;
  expr sState1;

  void xorShift128PlusSymbolic();
  uint64_t xorShift128PlusConcreteBackwards();
  uint64_t recoverMantissa(double value);
  double toDouble(uint64_t value);
};
