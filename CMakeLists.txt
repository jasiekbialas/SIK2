cmake_minimum_required(VERSION 3.13)
project(zadanie2)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(zadanie2
        main.cpp
        parameters.cpp
        parameters.h
        shoutcast_request.cpp
        shoutcast_request.h net_interface.cpp net_interface.h connection_manager.cpp connection_manager.h)
