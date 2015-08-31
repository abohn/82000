#ifndef BigInt_hpp
#define BigInt_hpp

#include <gmp.h>

/// Wrapper around gmp's c-style integer class
class BigInt {
 public:
  BigInt();
  BigInt(const unsigned long kNum);
  BigInt(const unsigned long kBase, const unsigned long kExp);
  ~BigInt();

  BigInt(const BigInt& kCopy);
  BigInt(BigInt&& other);

  BigInt& operator=(const BigInt& kRhs);

  void swap(BigInt& other);

  // comparisons
  bool operator==(const BigInt& kOther) const { return mNum == kOther.mNum; }
  bool operator!=(const BigInt& kOther) const { return !(*this == kOther); }
  bool operator<(const BigInt& kOther) const {
    return mpz_cmp(mNum, kOther.mNum) < 0;
  }
  bool operator<=(const BigInt& kOther) const {
    return mpz_cmp(mNum, kOther.mNum) <= 0;
  }
  bool operator>(const BigInt& kOther) const { return !(*this <= kOther); }
  bool operator>=(const BigInt& kOther) const { return !(*this < kOther); }
  bool operator<(const unsigned long kInt) const {
    return mpz_cmp_ui(mNum, kInt) < 0;
  }
  bool operator<=(const unsigned long kInt) const {
    return mpz_cmp_ui(mNum, kInt) <= 0;
  }
  bool operator>(const unsigned long kInt) const { return !(*this <= kInt); }
  bool operator>=(const unsigned long kInt) const { return !(*this < kInt); }

  // set value operations
  void Set(const unsigned long kValue);
  void Set(const BigInt& kValue);
  void Set(const unsigned long kBase, const unsigned long kExponent);

  // math
  BigInt& operator+=(const BigInt& kRhs);
  BigInt& operator-=(const BigInt& kRhs);
  BigInt& operator*=(const BigInt& kRhs);
  /// division must have no remainder to get proper results.  There is no safety
  /// check, for speed
  BigInt& operator/=(const unsigned long kRhs);

  void RaiseToPower(const unsigned long kExponent);
  /// raises to an exponent, then mods the result
  void RaiseToPower(const unsigned long kExponent, const BigInt& kModulus);

  /// Computes roughly the number of digits to represent the absolute value of
  /// the number in the specified base, but is only exact if the base is a power
  /// of 2.
  /// base must satisfy 2 <= base <= 62
  size_t SizeInBase(const size_t kBase) const {
    return mpz_sizeinbase(mNum, kBase);
  }

  /// Appends the number and a new line to the specified file
  void PrintToFile(const std::string& kFile) const;

 private:
  mpz_t mNum;
};

#endif
