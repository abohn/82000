#include "TestNumber.hpp"

#include "Tools/BigInt.hpp"

bool TestNumber(BigInt& num, const uint8_t kBase, BigInt& check, BigInt& temp) {
  // Test num starting from the largest digit in kBase
  // The check variable stores kBase^n, or 1 followed by 0s in base kBase, and
  // corresponds to the digit in num we're currently testing
  check.Set(kBase, num.SizeInBase(kBase));
  temp.Set(num);

  // Stores the number of ones in a row since a 0, in the order that digits are
  // checked in this function. Useful upon failure for guessing the next number
  auto consecutiveOnesSinceZero = uint64_t{0};
  auto valid = true;

  while (check >= 1) {
    if (temp >= check) {
      // check is currently at the largest digit. Subtract the check and make
      // sure the largest digit is 1
      temp -= check;
      if (temp >= check) {
        // The largest digit was 2 or larger, so this fails
        valid = false;
        break;
      }
      consecutiveOnesSinceZero++;
    } else {
      consecutiveOnesSinceZero = 0;
    }
    if (check > 1) {
      // check goes from kBase^n to kBase^(n-1), removing a zero in base kBase
      check /= kBase;
    } else {
      check = 0;
    }
  }

  if (!valid) {
    // Skipping logic similar to reddit's u/grencez, but takes into account
    // consecutive ones before the failed digit properly
    // We need to generate the next largest number above num which is
    // represented by 0s and 1s
    // num  = 10111200300
    // temp =      100300
    // check=      100000
    // consecutiveOnesSinceZero = 3
    // next = 11000000000
    // In general, we have:
    num -= temp;
    num -= check;
    for (size_t i = consecutiveOnesSinceZero - 1; i < consecutiveOnesSinceZero;
         --i) {
      check *= kBase;
      num -= check;
    }
    check *= kBase;
    num += check;
  }

  return valid;
}
