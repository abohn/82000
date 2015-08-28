#include <cmath>
#include <iostream>

#include "Tools/BigInt.hpp"

// Find the smallest number whose representation in all bases below a specified
// base only uses 0s and 1s

// Known numbers of the sequence are 2,3,4,82000, corresponding to bases 2,3,4,5
// and lower
// base <= 2: 2. 10 in base 2 (10b2)
// base <= 3: 3. 11b2, 10b3
// base <= 4: 4. 100b2, 11b3, 10b4
// base <= 5: 82000. Note that 5 doesn't work, since 5b10 = 12b3

/// Tests if the passed number has only 0s and 1s in the specified base
/// representation.
/// The number will be modified to be equal to the next largest number which
/// satisfies the test in this base
bool TestNumber(BigInt& num, const size_t kBase) {
  // Test num starting from the largest digit in kBase
  // The check variable stores kBase^n, or 1 followed by 0s in base kBase, and
  // corresponds to the digit in num we're currently testing
  BigInt check(kBase, num.SizeInBase(kBase));
  BigInt temp(num);

  // holds if the last successful digit was 0 or 1 in base kBase
  bool lastDigitWasOne = false;
  bool valid = true;
  while (check > 1) {
    if (temp >= check) {
      // check is currently at the largest digit. Subtract the check and make
      // sure the largest digit is 1
      temp -= check;
      if (temp >= check) {
        // The largest digit was 2 or larger, so this fails
        valid = false;
        break;
      }
      lastDigitWasOne = true;
    }
    else {
      lastDigitWasOne = false;
    }
    // check goes from kBase^n to kBase^(n-1), removing a zero in base kBase
    check /= kBase;
  }

  if (!valid) {
    // We need to generate the next largest number above num which is
    // represented by 0s and 1s
    // There are two possibilities, where the last good digit was 0 or 1
    // num  = 1000200300
    // lastdigitOne = false
    // temp =     100300
    // check=     100000
    // next = 1001000000
    //
    // or
    // num  = 1001200300
    // temp =     100300
    // check=     100000
    // lastdigitOne = true
    // next = 1010000000
    // In general, we have:
    num -= temp;
    num -= check;
    if (lastDigitWasOne) {
      check *= kBase;
      num -= check;
    }
    check *= kBase;
    num += check;
  }

  return valid;
}

int main() {
  // Some configurables
  const size_t kMinBase = 3;
  const size_t kMaxBase = 5;
  const size_t kStartNumLog10 = 1;

  static_assert(kStartNumLog10 >= 1, "Must start search at 10^1 or above");
  static_assert(kMinBase <= kMaxBase, "Min base must be <= max base");
  static_assert((kMaxBase == 5 && kStartNumLog10 <= 4) || kMaxBase >= 6,
                "Start of search is larger than solution for max base");

  std::cout << "Finding smallest number represented by only 0s and 1s from\n"
    << "base " << kMinBase << " to " << kMaxBase << ", larger than "
    << "10^" << kStartNumLog10 << "\n";

  BigInt answer();
  bool foundAnswer = false;
  BigInt guess(10);
  guess.RaiseToPower(kStartNumLog10);

  while (!foundAnswer) {
    bool guessIsCorrect = true;

    // Bail out of the loop as soon as one base fails the test.
    // Start with the maximum base, so statistically our next guess will be
    // the farthest away
    for (size_t iBase = kMaxBase; iBase >= kMinBase && guessIsCorrect;
         iBase--) {
      guessIsCorrect = TestNumber(guess, iBase);
    }
    foundAnswer = guessIsCorrect;
  }

  std::cout << "Answer written to answer.txt\n";
  guess.PrintToFile("answer.txt");
  return 0;
}
