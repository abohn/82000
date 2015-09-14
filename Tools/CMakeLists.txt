set(Tools_SOURCES
  BigInt.cpp
  TestHelper.cpp
  TestNumber.cpp
)

add_library(Tools ${Tools_SOURCES})

add_subdirectory(Tests)
