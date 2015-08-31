#include <cmath>
#include <iostream>
#include <omp.h>

#include "Tools/BigInt.hpp"

bool TestNumber(BigInt& num, const size_t kBase, BigInt& check, BigInt& temp);
double GetTimeInSeconds();

// Find the smallest number whose representation in all bases below a specified
// base only uses 0s and 1s

// Known numbers of the sequence are 2,3,4,82000, corresponding to bases 2,3,4,5
// and lower
// base <= 2: 2. 10 in base 2 (10b2)
// base <= 3: 3. 11b2, 10b3
// base <= 4: 4. 100b2, 11b3, 10b4
// base <= 5: 82000. Note that 5 doesn't work, since 5b10 = 12b3
int main() {
  // Some configurables
  const size_t kMinBase = 3;
  const size_t kMaxBase = 6;
  const uint64_t kStartNumLog10 = 1;

  static_assert(kStartNumLog10 >= 1, "Must start search at 10^1 or above");
  static_assert(kMinBase <= kMaxBase, "Min base must be <= max base");
  static_assert((kMaxBase == 5 && kStartNumLog10 <= 4) || kMaxBase >= 6,
                "Start of search is larger than solution for max base");

  std::cout << "Finding smallest number represented by only 0s and 1s from\n"
            << "base " << kMinBase << " to " << kMaxBase << ", larger than "
            << "10^" << kStartNumLog10 << "\n";

  const double kStart = GetTimeInSeconds();

  BigInt answer;
  BigInt guess(10);
  guess.RaiseToPower(kStartNumLog10);

  bool foundAnswer = false;
  // Each thread will check a range of numbers from currentNumBits to
  // currentNumBits + kNumBitsToCheck, before grabbing the next available
  // range of numbers
  uint64_t currentNumBits = guess.SizeInBase(2);
  // check this many bits, which has log(2)/log(10) times as many digits in base
  // 10
  // We may want to explore making this proportional to the size of the current
  // guess, since the guesses should jump proportionally
  const uint64_t kNumBitsToCheck = 1e3;

  uint numsCheckedThisRange = 0;

#ifdef _OPENMP
#pragma omp parallel default(none) private( \
    guess, numsCheckedThisRange)            \
        shared(std::cout, currentNumBits, foundAnswer, answer)
#endif
  {
    uint64_t upperBoundNumBitsToCheck;
    // temporary variables used in TestNumber to minimize memory allocations
    BigInt temp1, temp2;

    while (!foundAnswer) {
#ifdef _OPENMP
#pragma omp critical(getBitRange)
#endif
      {
        guess.Set(2, currentNumBits);
        currentNumBits += kNumBitsToCheck;
        upperBoundNumBitsToCheck = currentNumBits;
      }

// Print out progress
#ifdef _OPENMP
#pragma omp master
#endif
      {
        std::cout << guess.SizeInBase(10) << " " << guess.SizeInBase(2) << " "
                  << (GetTimeInSeconds() - kStart) << " "
                  << numsCheckedThisRange << "\n";
      }
      numsCheckedThisRange = 0;

      while (!foundAnswer && guess.SizeInBase(2) < upperBoundNumBitsToCheck) {
        bool guessIsCorrect = true;
        numsCheckedThisRange++;

        // Bail out of the loop as soon as one base fails the test.
        // Start with the maximum base, so statistically our next guess will be
        // the farthest away
        for (size_t iBase = kMaxBase; iBase >= kMinBase && guessIsCorrect;
             iBase--) {
          guessIsCorrect = TestNumber(guess, iBase, temp1, temp2);
        }

        if (guessIsCorrect) {
#ifdef _OPENMP
#pragma omp critical(updateAnswer)
#endif
          {
            if (foundAnswer) {
              // We've already found an answer, so we need to choose the smaller
              // of the two
              std::cout
                  << "Found another answer.  Saving the smaller of the two\n";
              if (guess < answer) {
                answer = guess;
              }
            } else {
              std::cout << "Found an answer\n";
              foundAnswer = true;
              answer = guess;
            }
          }
        }
      }
    }
  }  // end omp parallel section

  std::cout << "Answer written to answer.txt\n";
  answer.PrintToFile("answer.txt");
  return 0;
}

/// Tests if the passed number has only 0s and 1s in the specified base
/// representation.
/// The number will be modified to be equal to the next largest number which
/// satisfies the test in this base
/// The check variable and temp variable are temporaries passed in to minimize
/// memory allocations
bool TestNumber(BigInt& num, const size_t kBase, BigInt& check, BigInt& temp) {
  // Test num starting from the largest digit in kBase
  // The check variable stores kBase^n, or 1 followed by 0s in base kBase, and
  // corresponds to the digit in num we're currently testing
  check.Set(kBase, num.SizeInBase(kBase));
  temp.Set(num);

  // Stores the number of ones in a row since a 0, in the order that digits are
  // checked in this function. Useful upon failure for guessing the next number
  size_t consecutiveOnesSinceZero = 0;
  bool valid = true;
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

/// Uses correct time function when OpenMP is being used
double GetTimeInSeconds() {
  double time;
#ifdef _OPENMP
  time = omp_get_wtime();
#else
  struct timeval tv;
  gettimeofday(&tv, nullptr);
  time = tv.tv_sec + tv.tv_usec / (1.e6);
#endif
  return time;
}
