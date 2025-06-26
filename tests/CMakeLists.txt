add_executable(tests test_main.cpp)
target_link_libraries(tests gtest gtest_main)
add_test(NAME tests COMMAND tests)
