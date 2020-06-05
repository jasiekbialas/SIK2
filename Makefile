CXX = g++
CPPFLAGS = -g -std=c++17 -Wall -Wextra -O2
LDFLAGS=-g
LDLIBS=



COMMON = ./common/parameters.cpp ./common/local_interface.cpp ./common/local_message.cpp
SERVER = ./server/server.cpp ./server/shoutcast_request.cpp ./server/radio_interface.cpp ./server/radio_manager.cpp ./server/server_manager.cpp ./server/server_interface.cpp ./server/server_parameters.cpp $(COMMON)
CLIENT = ./client/client.cpp ./client/client_interface.cpp ./client/client_manager.cpp ./client/user_interface.cpp ./client/telnet_interfaces.cpp ./client/client_parameters.cpp $(COMMON)

all: radio-proxy radio-client

radio-proxy:  $(SERVER:%.cpp=%.o)
	$(CXX) $(CPPFLAGS) -o radio-proxy $(SERVER:%.cpp=%.o)

radio-client:  $(CLIENT:%.cpp=%.o)
	$(CXX) $(CPPFLAGS) -o radio-client $(CLIENT:%.cpp=%.o)

clean:
	rm -f ./common/*.o
	rm -f ./server/*.o
	rm -f ./client/*.o
	rm radio-proxy
	rm radio-client
