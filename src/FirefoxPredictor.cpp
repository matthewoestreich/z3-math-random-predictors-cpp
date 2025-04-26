#include "FirefoxPredictor.hpp"

#include <z3++.h>

FirefoxPredictor::FirefoxPredictor(const std::vector<double> &sequence)
    : context(),
      solver(context),
      sState0(context.bv_const("se_state0", 64)),
      sState1(context.bv_const("se_state1", 64)),
      kDoubleSignificantMaskSymbolic(
          context.bv_val(kDoubleSignificantMask, 64)) {
  this->sequence = sequence;

  for (double observed : this->sequence) {
    uint64_t mantissa = recoverMantissa(observed);
    xorShift128PlusSymbolic();
    solver.add(context.bv_val(mantissa, 64) ==
               ((sState0 + sState1) & kDoubleSignificantMaskSymbolic));
  }

  if (solver.check() != z3::sat) {
    throw std::runtime_error("UNSAT");
  }

  z3::model model = solver.get_model();
  cState0 = model.eval(sState0).as_uint64();
  cState1 = model.eval(sState1).as_uint64();
}

double FirefoxPredictor::predictNext() {
  return toDouble(xorShift128PlusConcrete());
}

void FirefoxPredictor::xorShift128PlusSymbolic() {
  z3::expr s1 = sState0;
  z3::expr s0 = sState1;
  s1 = s1 ^ z3::shl(s1, 23);
  s1 = s1 ^ z3::lshr(s1, 17);
  s1 = s1 ^ s0;
  s1 = s1 ^ z3::lshr(s0, 26);
  sState0 = sState1;
  sState1 = s1;
}

uint64_t FirefoxPredictor::xorShift128PlusConcrete() {
  uint64_t s1 = cState0;
  uint64_t s0 = cState1;
  s1 = s1 ^ (s1 << 23);
  s1 = s1 ^ (s1 >> 17);
  s1 = s1 ^ s0;
  s1 = s1 ^ (s0 >> 26);
  cState0 = s0;
  cState1 = s1;
  return (cState0 + cState1);
}

uint64_t FirefoxPredictor::recoverMantissa(double value) {
  return static_cast<uint64_t>(value * (1ULL << kDoubleSignificantBits));
}

double FirefoxPredictor::toDouble(uint64_t value) {
  return static_cast<double>(value & kDoubleSignificantMask) /
         (1ULL << kDoubleSignificantBits);
}
