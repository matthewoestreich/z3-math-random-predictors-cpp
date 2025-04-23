#include <algorithm>
#include <cstdint>
#include <cstring>
#include <bit>
#include <iostream>
#include <vector>
#include <z3++.h>

using namespace std;
using namespace z3;

class V8Predictor {
private:
  const uint64_t mask = 0xFFFFFFFFFFFFFFFF;
  vector<double> sequence;
  uint64_t cState0;
  uint64_t cState1;
  context context;
  solver solver;
  expr sState0;
  expr sState1;

  void xorShift128PlusSymbolic() {
    expr s1 = sState0;
    expr s0 = sState1;
    sState0 = s0;
    s1 = s1 ^ (s1 << 23);
    s1 = s1 ^ lshr(s1, 17);
    s1 = s1 ^ s0;
    s1 = s1 ^ lshr(s0, 26);
    sState1 = s1;
    int x = 1;
  }

  uint64_t xorShift128PlusConcreteBackwards() {
    uint64_t result = cState0;
    uint64_t ps1 = cState0;
    uint64_t ps0 = cState1 ^ (cState0 >> 26);
    ps0 = ps0 ^ cState0;
    ps0 = ps0 ^ (ps0 >> 17) ^ (ps0 >> 34) ^ (ps0 >> 51);
    ps0 = (ps0 ^ (ps0 << 23) ^ (ps0 << 46)) & mask;
    cState0 = ps0;
    cState1 = ps1;
    return result;
  }

  uint64_t recoverMantissa(double value) {
    return bit_cast<uint64_t>(value) & ((1ULL << 52) - 1);
  }

  double toDouble(uint64_t value) {
   return bit_cast<double>((value >> 12) | 0x3FF0000000000000) - 1;
  }

public:
  V8Predictor(vector<double> sequence) :
        context(),
        solver(context),
        sState0(context.bv_const("se_state0", 64)),
        sState1(context.bv_const("se_state1", 64)) {
    this->sequence = sequence;
    reverse(this->sequence.begin(), this->sequence.end());

    for (double observed : this->sequence) {
      xorShift128PlusSymbolic();
      uint64_t mantissa = recoverMantissa(observed + 1);
      solver.add(context.bv_val(mantissa, 64) == lshr(sState0, 12));
    }

    if (solver.check() != sat) {
      return;
    }

    model model = solver.get_model();
    cState0 = model.eval(sState0).as_uint64();
    cState1 = model.eval(sState1).as_uint64();
  }

  double predictNext() {
    if (!cState0 || !cState1) {
      throw invalid_argument("Concrete state(s) undefined!");
    }
    uint64_t out = xorShift128PlusConcreteBackwards();
    return toDouble(out);
  }
};
