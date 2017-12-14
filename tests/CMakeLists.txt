cmake_minimum_required (VERSION 2.8.11)
project (CppMonadTest)

FILE(GLOB TestCases ./Cases/*cpp)
add_executable(${PROJECT_NAME} ${TestCases} Main.cpp)

include_directories(../include)
#set(CMAKE_CXX_COMPILER "g++-7")
set(CMAKE_CXX_FLAGS "-O3 -g --std=c++17")
