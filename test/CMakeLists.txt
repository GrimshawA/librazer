file(GLOB_RECURSE test_src
    "src/main_tests.cpp"
    "src/*.cpp"
    "src/*.h*"
)

add_executable(all_test ${test_src})
target_link_libraries(all_test librazer gtest)