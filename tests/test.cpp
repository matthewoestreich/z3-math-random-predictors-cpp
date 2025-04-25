#define CATCH_CONFIG_MAIN

#include "FirefoxPredictor.hpp"
#include "V8Predictor.hpp"
#include <catch2/catch_all.hpp>

TEST_CASE("FirefoxPredictor", "[FirefoxPredictor]") {
  std::vector<double> sequence{
      0.1321263101773572, 0.03366887439746058, 0.032596957696410134, 0.9986575482138969, 0.8479779907956815,
  };
  std::vector<double> expected{
      0.13963871472821332, 0.25068024611907636, 0.6656237481612675, 0.7381091878692425, 0.8709382509549467,
      0.49171337524788294, 0.6991749430716799,  0.9530887478758369, 0.781511163650037,  0.699311162730038,
  };

  SECTION(std::format("correctly predicts the next {} elements", expected.size())) {
    FirefoxPredictor firefox(sequence);
    std::vector<double> predictions;

    for (double expect : expected) {
      predictions.push_back(firefox.predictNext());
    }

    REQUIRE(expected == predictions);
  }
}

TEST_CASE("V8Predictor", "[V8Predictor]") {
  std::vector<double> sequence{
      0.5444405645692001, 0.9203027285129253, 0.0880920246518424, 0.6555723374589169, 0.06183370879928152,
  };
  std::vector<double> expected{
      0.3817506775921966, 0.796686249606257, 0.9976624647084931, 0.39031891128710483, 0.7825669445360215,
  };

  SECTION(std::format("correctly predicts the next {} elements", expected.size())) {
    V8Predictor v8(sequence);
    std::vector<double> predictions;

    for (double expect : expected) {
      predictions.push_back(v8.predictNext());
    }

    REQUIRE(expected == predictions);
  }
}
