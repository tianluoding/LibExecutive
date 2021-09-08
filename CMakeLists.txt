cmake_minimum_required(VERSION 2.8)

project(TESTBUF)

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

include_directories(${PROJECT_SOURCE_DIR}/include/)

add_library(mylog SHARED src/CLStatus.cpp src/CLLogger.cpp )

add_executable(testbuf src/testbuf.cpp)

target_link_libraries(testbuf mylog)