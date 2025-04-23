#include <iostream>
#include <vector>
#include <z3++.h>
#include "V8Predictor.h"

using namespace std;

int main() {
  vector<double> sequence{
      0.552974198778651,  0.6091405699404759,  0.30377761820864757,
      0.6615235206792083, 0.27808781139439565,
  };

  vector<double> expected{
      0.4343553191734457, 0.22840345766161696, 0.5582855770529151,
      0.50415840256704,   0.5362054737043922,
  };

  V8Predictor predictor(sequence);

  for (double expect : expected) {
    double next = predictor.predictNext();
    bool correct = expect == next;
    cout << "Correct? " << correct << " expected= " << expect << " got= " << next << "\n";
  }

  return 0;
}