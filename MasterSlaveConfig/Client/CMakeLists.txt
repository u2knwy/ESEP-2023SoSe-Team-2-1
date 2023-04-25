cmake_minimum_required(VERSION 3.25)
project(Client)

set(CMAKE_CXX_STANDARD 14)

include_directories(helloworld/src/comm)
include_directories(helloworld/src/hal)
include_directories(helloworld/src/utils)
include_directories(C:/Users/Dominik/qnx700/target/qnx7/usr/include)

add_executable(Client
        helloworld/src/comm/Communication.cpp
        helloworld/src/comm/Communication.h
        helloworld/src/hal/hal.cpp
        helloworld/src/hal/hal.h
        helloworld/src/utils/cxxopts.hpp
        helloworld/src/helloworld.cpp)
