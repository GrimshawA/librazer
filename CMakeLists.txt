cmake_minimum_required(VERSION 2.8.11)
project (RazerLanguage)

#PYTHON_INCLUDE_DIR=/usr/include/python2.7 
#PYTHON_LIBRARY=/usr/lib/python2.7/config/libpython2.7.so

################################
# GTest
################################
set (gtest_SOURCE_DIR 3rdparty/googletest)
ADD_SUBDIRECTORY (3rdparty/googletest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set (CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# The standard SDK
add_subdirectory(sdk)
add_subdirectory("3rdparty/sdl2")

file(GLOB_RECURSE lib_SRC
    "src/Base/*.h"
    "src/Base/*.cpp"
    "src/RazerBuild/*.h"
    "src/RazerBuild/*.cpp"
    "src/RazerCompiler/*.h*"
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

file(GLOB_RECURSE rz_SRC
    "src/rz/*.h*"
    "src/rz/*.cpp"
)

file(GLOB files ${lib_SRC} ${sdk_files})

include_directories("src/" "sdk/src/")

add_library(librazer SHARED ${files} src/razer/bind_utils.h)
target_compile_definitions(librazer PRIVATE AVC=1 )

add_definitions("-DRAZER_SDL")
target_link_libraries(librazer SDL2)

if(WIN32)
  target_link_libraries(librazer wsock32 ws2_32)
  target_link_libraries(librazer winmm.lib)
else()
  target_link_libraries(librazer pthread)
endif()

# Compiler
#add_executable(rzc src/rzc/main.cpp)
#target_link_libraries(rzc librazer)

# Razer VM
add_executable(rz ${rz_SRC})
target_link_libraries(rz librazer)

add_subdirectory(test)






