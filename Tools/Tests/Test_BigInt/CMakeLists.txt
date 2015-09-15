add_executable(Test_BigInt Test_BigInt.cpp)

add_test(Test_BigInt Test_BigInt)

target_link_libraries(
  Test_BigInt
  Tools
  gmp
)
