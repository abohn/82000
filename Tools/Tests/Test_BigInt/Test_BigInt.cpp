#include "Tools/BigInt.hpp"
#include "Tools/TestHelper.hpp"

#include <cmath>

int main() {
  // test ctors
  // compare default ctor to passing an integer in
  auto zero = BigInt{0};
  TEST(zero == BigInt(), "Default ctor should initialize to 0");
  auto ctorTest = BigInt{7, 9};
  TEST(ctorTest == BigInt{static_cast<unsigned long>(pow(7, 9))},
       "base^exp ctor doesn't match integer ctor");
  auto copy = BigInt{ctorTest};
  TEST(copy == ctorTest,
       "Copy constructor value doesn't match the value it copied");

  auto assign = copy;
  TEST(copy == assign, "Assignment operator incorrect");

  // comparisons with BigInt
  TEST(copy != zero, "!= operator");
  TEST(copy <= assign, "copy == assign");
  TEST(copy >= assign, "copy == assign");
  TEST(copy > zero, "7^9 > 0");
  TEST(copy >= zero, "7^9 >= 0");
  TEST(zero < copy, "0 < 7^9");
  TEST(zero <= copy, "0 <= 7^9");

  // comparisons with int
  TEST(copy != 5, "BigInt != int");
  TEST(zero == 0, "== int");
  TEST(copy > 5, "> int");
  TEST(copy >= 5, ">= int failed when BigInt was < int");
  TEST(zero >= 0, ">= int failed when BigInt was == int");
  TEST(zero < 5, "< int");
  TEST(zero <= 5, "<= int failed when BigInt was < int");
  TEST(zero <= 0, "<= int failed when BigInt was == int");

  // set values
  auto num = BigInt{0};
  TEST(num == 0, "sanity check");
  num.Set(1);
  TEST(num == 1, "Set int");
  num.Set(2);
  TEST(num == 2, "Set int  the second time");
  num.Set(2, 3);
  TEST(num == pow(2, 3), "Set to base^exponent");
  num.Set(zero);
  TEST(num == 0, "Set to BigInt");

  // math
  num = 0;
  num += BigInt(2);
  TEST(num == 2, "+=");
  num = 2;
  num -= BigInt(1);
  TEST(num == 1, "-=");
  num = 1;
  num *= 7;
  TEST(num == 7, "*=");
  num = 10;
  num /= 5;  // division must be exact!
  TEST(num == 2, "exact /=");
  num = 2;
  num.RaiseToPower(3);
  TEST(num == 8, "RaiseToPower");
  num = 2;
  num.RaiseToPower(4, 3);
  TEST(num == 1, "Raise to power and mod");

  return TestHelper::Summary();
}
