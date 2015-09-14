add_executable(Test_TestNumber Test_TestNumber.cpp)

add_test(Test_TestNumber Test_TestNumber)

target_link_libraries(
  Test_TestNumber
  Tools
  gmp
)
