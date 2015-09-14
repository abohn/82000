#include <cmath>
#include <iostream>
#include <omp.h>

#include "Tools/BigInt.hpp"
#include "Tools/TestNumber.hpp"

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
  const auto kMinBase = uint8_t{3};
  const auto kMaxBase = uint8_t{6};
  const auto kStartNumLog10 = uint64_t{8400000};

  static_assert(kStartNumLog10 >= 1, "Must start search at 10^1 or above");
  static_assert(kMinBase <= kMaxBase, "Min base must be <= max base");
  static_assert((kMaxBase == 5 && kStartNumLog10 <= 4) || kMaxBase >= 6,
                "Start of search is larger than solution for max base");

  std::cout << "Finding smallest number represented by only 0s and 1s from\n"
            << "base " << kMinBase << " to " << kMaxBase << ", larger than "
            << "10^" << kStartNumLog10 << std::endl;

  const auto kStart = GetTimeInSeconds();

  auto answer = BigInt{};
  auto guess = BigInt{10};
  guess.RaiseToPower(kStartNumLog10);

  auto foundAnswer = false;
  // Each thread will check a range of numbers from currentNumBits to
  // currentNumBits + kNumBitsToCheck, before grabbing the next available
  // range of numbers
  auto currentNumBits = guess.SizeInBase(2);
  // check this many bits, which has log(2)/log(10) times as many digits in base
  // 10
  // We may want to explore making this proportional to the size of the current
  // guess, since the guesses should jump proportionally
  const auto kNumBitsToCheck = 1e3;
  auto numsCheckedThisRange = uint32_t{0};

#ifdef _OPENMP
#pragma omp parallel default(none) private( \
    guess, numsCheckedThisRange)            \
        shared(std::cout, currentNumBits, foundAnswer, answer)
#endif
  {
    auto upperBoundNumBitsToCheck = uint64_t{0};
    // temporary variables used in TestNumber to minimize memory allocations
    auto temp1 = BigInt{}, temp2 = BigInt{};

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
                  << numsCheckedThisRange << std::endl;
      }
      numsCheckedThisRange = 0;

      // TestNumber's next guess satisfies the last base it checked.
      // We save a lot by skipping the check in that base
      auto skipBase = decltype(kMinBase){0};
      while (!foundAnswer && guess.SizeInBase(2) < upperBoundNumBitsToCheck) {
        auto guessIsCorrect = true;
        numsCheckedThisRange++;

        // Bail out of the loop as soon as one base fails the test.
        // Start with the maximum base, so statistically our next guess will be
        // the farthest away
        for (auto iBase = kMaxBase; iBase >= kMinBase && guessIsCorrect;
             iBase--) {
          if (iBase != skipBase) {
            guessIsCorrect = TestNumber(guess, iBase, temp1, temp2);
          }

          if (!guessIsCorrect) {
            skipBase = iBase;
          }
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


/// Uses correct time function when OpenMP is being used
double GetTimeInSeconds() {
  auto time = 0.;
#ifdef _OPENMP
  time = omp_get_wtime();
#else
  auto tv = timeval{};
  gettimeofday(&tv, nullptr);
  time = tv.tv_sec + tv.tv_usec / (1.e6);
#endif
  return time;
}
