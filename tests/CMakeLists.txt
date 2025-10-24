add_executable(
  unit_tests
  lexer.cpp
  parser.cpp
)
target_link_libraries(
  unit_tests
  GTest::gtest_main
  virtual-machine compiler parser lexer shared
)

include(GoogleTest)
gtest_discover_tests(unit_tests)
