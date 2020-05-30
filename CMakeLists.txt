cmake_minimum_required(VERSION 3.13)
project(zadanie2)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(zadanie2
        main.cpp
        parameters.cpp
        parameters.h
        shoutcast_request.cpp
        shoutcast_request.h net_interface.cpp net_interface.h connection_manager.cpp connection_manager.h local_interface.cpp local_interface.h proxy_manager.cpp proxy_manager.h)
