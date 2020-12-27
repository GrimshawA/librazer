

#file(GLOB_RECURSE test_src
#    "src/main_tests.cpp"
#    "src/*.cpp"
#    "src/*.h*"
#)

#add_executable(all_test ${test_src})
#target_link_libraries(all_test librazer gtest)

# integration tests

file(GLOB_RECURSE integration_tests_sources
    "integration_test/*cpp"
)
add_executable(integration_tests ${integration_tests_sources})
target_link_libraries(integration_tests librazer)
target_include_directories(integration_tests PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/../3rdparty/catch2)
