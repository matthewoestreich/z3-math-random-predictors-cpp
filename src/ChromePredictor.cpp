#include "ChromePredictor.hpp"
#include <z3++.h>

ChromePredictor::ChromePredictor(const std::vector<double> &sequence) :
      context(),
      solver(context),
      sState0(context.bv_const("se_state0", 64)),
      sState1(context.bv_const("se_state1", 64)) {
  this->sequence = sequence;
  reverse(this->sequence.begin(), this->sequence.end());

  for (double observed : this->sequence) {
    xorShift128PlusSymbolic();
    uint64_t mantissa = recoverMantissa(observed);
    solver.add(context.bv_val(mantissa, 64) == lshr(sState0, 11));
  }

  if (solver.check() != z3::sat) {
    throw std::runtime_error("UNSAT");
  }

  z3::model model = solver.get_model();
  cState0 = model.eval(sState0).as_uint64();
  cState1 = model.eval(sState1).as_uint64();

  // We need to catch concrete state up to symbolic state
  for (size_t i = 0; i < this->sequence.size(); ++i) {
    xorShift128PlusConcrete();
  }
}

double ChromePredictor::predictNext() {
  return toDouble(xorShift128PlusConcrete());
}

void ChromePredictor::xorShift128PlusSymbolic() {
  z3::expr s1 = sState0;
  z3::expr s0 = sState1;
  s1 = s1 ^ z3::shl(s1, 23);
  s1 = s1 ^ z3::lshr(s1, 17);
  s1 = s1 ^ s0;
  s1 = s1 ^ z3::lshr(s0, 26);
  sState0 = sState1;
  sState1 = s1;
}

/**
 * Performs the concrete XOR backwards due to how Chrome provides random numbers.
 */
uint64_t ChromePredictor::xorShift128PlusConcrete() {
  uint64_t result = cState0;
  uint64_t ps1 = cState0;
  uint64_t ps0 = cState1 ^ (cState0 >> 26);
  ps0 = ps0 ^ cState0;
  ps0 = ps0 ^ (ps0 >> 17) ^ (ps0 >> 34) ^ (ps0 >> 51);
  ps0 = (ps0 ^ (ps0 << 23) ^ (ps0 << 46));
  cState0 = ps0;
  cState1 = ps1;
  return result;
}

uint64_t ChromePredictor::recoverMantissa(double value) {
  return static_cast<uint64_t>(value * (1ULL << 53));
}

double ChromePredictor::toDouble(uint64_t value) {
  return static_cast<double>(value >> 11) / (1ULL << 53);
}
