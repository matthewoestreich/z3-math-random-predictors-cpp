#include "V8Predictor.hpp"
#include <iostream>
#include <vector>
#include <z3++.h>

using namespace std;

int main() {
  vector<double> sequence{
      0.5444405645692001, 0.9203027285129253,  0.0880920246518424,
      0.6555723374589169, 0.06183370879928152,
  };

  vector<double> expected{
      0.3817506775921966,  0.796686249606257,  0.9976624647084931,
      0.39031891128710483, 0.7825669445360215,
  };

  try {
    V8Predictor predictor(sequence);

    for (double expect : expected) {
      double next = predictor.predictNext();
      string correct = expect == next ? "True" : "False";
      cout << "Correct? " << correct << "\t|\texpected= " << format("{}", expect)
           << "\t|\tgot= " << format("{}", next) << "\n";
    }
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  return 0;
}