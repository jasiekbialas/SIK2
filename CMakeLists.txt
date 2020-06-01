cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)
project(japierdole_sik)

include_directories(.)

add_executable(server
        server.cpp
        parameters.cpp
        parameters.h
        shoutcast_request.cpp
        shoutcast_request.h net_interface.cpp net_interface.h connection_manager.cpp connection_manager.h local_interface.cpp local_interface.h proxy_manager.cpp proxy_manager.h local_message.cpp local_message.h local_server.cpp local_server.h local_client.cpp local_client.h)


add_executable(client
        client.cpp
        parameters.cpp
        parameters.h
        shoutcast_request.cpp
        shoutcast_request.h net_interface.cpp net_interface.h connection_manager.cpp connection_manager.h local_interface.cpp local_interface.h proxy_manager.cpp proxy_manager.h local_message.cpp local_message.h local_server.cpp local_server.h local_client.cpp local_client.h)
