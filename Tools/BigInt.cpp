#include "BigInt.hpp"

#include <iostream>

BigInt::BigInt() { mpz_init(mNum); }

BigInt::BigInt(const unsigned long kNum) : BigInt() { Set(kNum); }

BigInt::BigInt(const unsigned long kBase, const unsigned long kExp) : BigInt() {
  Set(kBase, kExp);
}

BigInt::~BigInt() { mpz_clear(mNum); }

BigInt::BigInt(const BigInt& kCopy) {
  mpz_init(mNum);
  mpz_set(mNum, kCopy.mNum);
}

BigInt::BigInt(BigInt&& other) : BigInt() { this->swap(other); }

BigInt& BigInt::operator=(const BigInt& kRhs) {
  // Skip self-assignment
  if (this != &kRhs) {
    // copy and swap
    BigInt copy(kRhs);
    this->swap(copy);
  }
  return *this;
}

BigInt& BigInt::operator=(BigInt&& other) {
  this->swap(other);
  other.Set(0);
  return *this;
}

void BigInt::swap(BigInt& other) { mpz_swap(mNum, other.mNum); }

void BigInt::Set(const unsigned long kValue) { mpz_set_ui(mNum, kValue); }

void BigInt::Set(const BigInt& kValue) { mpz_set(mNum, kValue.mNum); }

void BigInt::Set(const unsigned long kBase, const unsigned long kExponent) {
  mpz_ui_pow_ui(mNum, kBase, kExponent);
}

BigInt& BigInt::operator+=(const BigInt& kRhs) {
  mpz_add(mNum, mNum, kRhs.mNum);
  return *this;
}

BigInt& BigInt::operator-=(const BigInt& kRhs) {
  mpz_sub(mNum, mNum, kRhs.mNum);
  return *this;
}

BigInt& BigInt::operator*=(const BigInt& kRhs) {
  mpz_mul(mNum, mNum, kRhs.mNum);
  return *this;
}

BigInt& BigInt::operator/=(const unsigned long kRhs) {
  mpz_divexact_ui(mNum, mNum, kRhs);
  return *this;
}

void BigInt::RaiseToPower(const unsigned long kExponent) {
  mpz_pow_ui(mNum, mNum, kExponent);
}

void BigInt::RaiseToPower(const unsigned long kExponent,
                          const BigInt& kModulus) {
  mpz_powm_ui(mNum, mNum, kExponent, kModulus.mNum);
}

void BigInt::PrintToFile(const std::string& kFile) const {
  FILE* pFile = fopen(kFile.c_str(), "a");
  if (pFile == nullptr) {
    std::cerr << "Cannot open file " << kFile << "\n";
    std::cerr << "Number has " << this->SizeInBase(10)
              << " or one fewer digit in base 10\n";
  } else {
    mpz_out_str(pFile, 10, mNum);
    fprintf(pFile, "\n");
    fclose(pFile);
  }
}
