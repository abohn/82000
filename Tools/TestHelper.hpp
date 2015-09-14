#ifndef TestHelper_hpp
#define TestHelper_hpp

#include <string>
#include <sstream>

#define TEST(x, msg)                     \
  {                                      \
    std::ostringstream ostr;             \
    ostr << msg;                         \
    TestHelper::TestWork(x, ostr.str()); \
  }

/// Defines a set of helper functions for tests, keeping track of the
/// statistics of the tests
class TestHelper {
 public:
  static size_t NumberOfTests() { return mNumTests; }
  static size_t NumberOfTestsPassed() { return mNumTests - mNumTestsFailed; }
  static size_t NumberOfTestsFailed() { return mNumTestsFailed; }

  /// If kEqual is false, print the message and increment numTestsFailed.
  /// This should be only called through the TEST macro
  static void TestWork(const bool kEqual, const std::string& kMessage);

  /// Prints the number of passed and failed tests, as well as returning the
  /// number of failed tests
  static size_t Summary();

 private:
  static size_t mNumTests;
  static size_t mNumTestsFailed;
};

#endif
