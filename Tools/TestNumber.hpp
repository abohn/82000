#include <cstdint>

class BigInt;

/// Tests if the passed number has only 0s and 1s in the specified base
/// representation.
/// The number will be modified to be equal to the next largest number which
/// satisfies the test in this base
/// The check variable and temp variable are temporaries passed in to minimize
/// memory allocations
bool TestNumber(BigInt& num, const uint8_t kBase, BigInt& check, BigInt& temp);
