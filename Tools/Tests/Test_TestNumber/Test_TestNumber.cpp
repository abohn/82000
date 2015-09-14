#include "Tools/BigInt.hpp"
#include "Tools/TestHelper.hpp"
#include "Tools/TestNumber.hpp"

#include <cmath>

int main() {
  auto temp1 = BigInt{}, temp2 = BigInt{};

  // Check the trivial case of 1 in bases 2-6
  // Note: We should reset the testNumber
  auto testNumber = BigInt{1};
  TEST(TestNumber(testNumber, 2, temp1, temp2) == true,
       "1 should be valid in base 2");
  testNumber = 1;
  TEST(TestNumber(testNumber, 3, temp1, temp2) == true,
       "1 should be valid in base 3");
  testNumber = 1;
  TEST(TestNumber(testNumber, 4, temp1, temp2) == true,
       "1 should be valid in base 4");
  testNumber = 1;
  TEST(TestNumber(testNumber, 5, temp1, temp2) == true,
       "1 should be valid in base 5");
  testNumber = 1;
  TEST(TestNumber(testNumber, 6, temp1, temp2) == true,
       "1 should be valid in base 6");

  // Check the null case, which should pass
  testNumber = 0;
  TEST(TestNumber(testNumber, 6, temp1, temp2) == true,
       "0 should be valid in base 6");
  testNumber = 0;
  TEST(TestNumber(testNumber, 3, temp1, temp2) == true,
       "0 should be valid in base 3");

  // Check a small case that fails some bases but passes others
  testNumber = 256;
  TEST(TestNumber(testNumber, 3, temp1, temp2) == true,
       "256 is 100111 in base 3, it should be valid");
  TEST(testNumber == 256, "The number should still be 256");
  testNumber = 256;
  TEST(TestNumber(testNumber, 4, temp1, temp2) == true,
       "256 is 10000 in base 4, it should be valid");
  TEST(testNumber == 256, "The number should still be 256");
  testNumber = 256;
  TEST(TestNumber(testNumber, 5, temp1, temp2) == false,
       "256 is 2011 in base 5, it should be invalid");
  TEST(testNumber == 625, "The next guess should move from 256 to 625");

  // Test a number that falls all bases, and their respective next guesses
  testNumber = 81998;
  TEST(TestNumber(testNumber, 5, temp1, temp2) == false,
       "81998 is 10110443 in base 5, it should be invalid");
  TEST(testNumber == 82000, "The next guess should move from 81998 to 82000");
  testNumber = 81998;
  TEST(TestNumber(testNumber, 4, temp1, temp2) == false,
       "81998 is 110001032 in base 4, it should be invalid");
  TEST(testNumber == 82000, "The next guess should move from 81998 to 82000");
  testNumber = 81998;
  TEST(TestNumber(testNumber, 3, temp1, temp2) == false,
       "81998 is 11011110222 in base 3, it should be invalid");
  TEST(testNumber == 81999, "The next guess should move from 81998 to 81999");

  // Test a number valid in bases 3-5, 82000
  testNumber = 82000;
  TEST(TestNumber(testNumber, 3, temp1, temp2) == true,
       "82000 is valid in base 3");
  TEST(testNumber == 82000, "The next guess should stay at 82000");
  testNumber = 82000;
  TEST(TestNumber(testNumber, 4, temp1, temp2) == true,
       "82000 is valid in base 4");
  TEST(testNumber == 82000, "The next guess should stay at 82000");
  testNumber = 82000;
  TEST(TestNumber(testNumber, 5, temp1, temp2) == true,
       "82000 is valid in base 5");
  TEST(testNumber == 82000, "The next guess should stay at 82000");

  // Test a large valid number in base 6
  testNumber = pow(6, 10) + pow(6, 7) + pow(6, 6) + pow(6, 2) + pow(6, 0);
  TEST(TestNumber(testNumber, 6, temp1, temp2) == true,
       "10011000101 in base 6 is valid in base 6");

  // A large invalid number in base 6
  testNumber = pow(6, 10) + pow(6, 7) + pow(6, 6) + pow(6, 5) + pow(6, 4)
               + pow(6, 3) + pow(6, 2) + 4 * pow(6, 1);
  TEST(TestNumber(testNumber, 6, temp1, temp2) == false,
       "10011111140 in base 6 should fail in base 6");
  // next number should be 10100000000
  TEST(testNumber == pow(6, 10) + pow(6, 8),
       "The next guess after 10011111140 should be 10100000000");

  return TestHelper::Summary();
}
