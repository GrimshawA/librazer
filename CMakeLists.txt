cmake_minimum_required(VERSION 2.8.11)
project (DaevaEngine)

set (CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")


file(GLOB_RECURSE lib_SRC
    "src/Base/*.h"
    "src/Base/*.cpp"
    "src/RazerBuild/*.h"
    "src/RazerBuild/*.cpp"
    "src/RazerCompiler/*.h"
    "src/RazerCompiler/*.cpp"
    "src/RazerCore/*.h"
    "src/RazerCore/*.cpp"
    "src/RazerParser/*.h"
    "src/RazerParser/*.cpp"
    "src/RazerRuntime/*.h"
    "src/RazerRuntime/*.cpp"
    "src/RazerVM/*.h"
    "src/RazerVM/*.cpp"
    "src/Logger.cpp"
)

include_directories("src/")

add_library(librazer SHARED ${lib_SRC})
target_compile_definitions(librazer PRIVATE AVC=1 )

if(WIN32)
  target_link_libraries(librazer wsock32 ws2_32)
  target_link_libraries(librazer winmm.lib)
endif()

# Compiler
add_executable(rzc src/rzc/main.cpp)
target_link_libraries(rzc librazer)

# Razer VM
add_executable(rz src/rz/main.cpp)
target_link_libraries(rz librazer)


