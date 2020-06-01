CXX = g++
CPPFLAGS = -g -std=c++17 -Wall -Wextra -O2
LDFLAGS=-g
LDLIBS=

SRCS= local_message.cpp local_interface.cpp net_interface.cpp connection_manager.cpp proxy_manager.cpp parameters.cpp shoutcast_request.cpp

OBJS=$(subst .cc,.o,$(SRCS))

local_message.o: local_message.cpp local_message.h

local_interface.o: local_interface.cpp local_interface.h local_message.h

net_interface.o: net_interface.cpp net_interface.h

connection_manager.o: connection_manager.cpp connection_manager.h net_interface.h

proxy_manager.o: proxy_manager.cpp proxy_manager.h local_interface.h

parameters.o: parameters.cpp parameters.h

shoutcast_request.o: shoutcast_request.cpp shoutcast_request.h parameters.h

server.o: server.cpp local_message.h local_interface.h net_interface.h connection_manager.h proxy_manager.h parameters.h shoutcast_request.h

client.o: server.cpp local_message.h local_interface.h net_interface.h connection_manager.h proxy_manager.h parameters.h shoutcast_request.h

server: server.o $(OBJS)
	$(CXX) $(CPPFLAGS) -o server server.o $(OBJS)

client: client.o $(OBJS)
	$(CXX) $(CPPFLAGS) -o client client.o $(OBJS)

clean:
	rm -f *.o $(TARGETS)
