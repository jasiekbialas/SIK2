cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS -O2)
project(sik_duze)

include_directories(.)

add_executable(server
        server/server.cpp
        server/server.h
        server/shoutcast_request.cpp
        server/radio_interface.cpp
        server/radio_manager.cpp
        server/server_manager.cpp
        server/server_interface.cpp
        server/server_parameters.cpp
        common/common.h
        common/local_message.cpp
        common/local_interface.cpp
        common/parameters.cpp)


add_executable(client
        client/client.cpp
        client/client.h
        client/client_interface.cpp
        client/client_manager.cpp
        client/user_interface.cpp
        client/telnet_interfaces.cpp
        client/client_parameters.cpp
        common/common.h
        common/parameters.cpp
        common/local_interface.cpp
        common/local_message.cpp common/common.cpp)
