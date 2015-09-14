#include "TestHelper.hpp"

#include <iostream>

size_t TestHelper::mNumTests = 0;
size_t TestHelper::mNumTestsFailed = 0;

void TestHelper::TestWork(const bool kEqual, const std::string& kMessage) {
  mNumTests++;
  if (!kEqual) {
    std::cerr << "Failed test:\n" << kMessage << "\n";
    mNumTestsFailed++;
  }
}

size_t TestHelper::Summary() {
  std::cout << "Passed " << NumberOfTestsPassed() << " out of "
            << NumberOfTests() << "\n";
  return NumberOfTestsFailed();
}
