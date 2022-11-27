# minimal version of cmake
cmake_minimum_required(VERSION 3.3)

# project name
project(reinforcement_learning)

# include directory path
include_directories(include)

# source files
file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

# type of compilation
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RELEASE)
endif()

# compilation options
set(CMAKE_CXX_FLAGS "-std=c++17")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -Wall")

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message("   Compiler CXX:         ${CMAKE_CXX_COMPILER}")
message("   Compiler CXX flags:         ${CMAKE_CXX_FLAGS}")
if(CMAKE_BUILD_TYPE MATCHES "DEBUG")
message("   Compiler CXX debug flags:   ${CMAKE_CXX_FLAGS_DEBUG}")
endif(CMAKE_BUILD_TYPE MATCHES "DEBUG")
if(CMAKE_BUILD_TYPE MATCHES "RELEASE")
message("   Compiler CXX release flags: ${CMAKE_CXX_FLAGS_RELEASE}")
endif(CMAKE_BUILD_TYPE MATCHES "RELEASE")

# create library librl.a
add_library(rl ${SOURCE_FILES})

# create executable
add_executable(run.e src/main.cpp)

# link libraries
target_link_libraries(run.e rl)
