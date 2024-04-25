cmake_minimum_required(VERSION 3.21)
project(CryptoChecker)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(crch src/main.cpp)
