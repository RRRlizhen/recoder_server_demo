all: record_server

CC=g++

CFLAGS= -g -std=c++11

LIBRARY= -L/usr/local/lib -lboost_system -lrtmp -lboost_chrono -lpthread -ljsoncpp

INCLUDE= -I./ -I/usr/local/include/websocketpp -I/usr/local/include/boost/ -I/usr/include/jsoncpp/ -I/home/lizhen/test/rtmpdump

record_server: record_server.o ParseConf.o RtmpServer.o ThreadPool.o User.o
	$(CC) -o record_server record_server.o ParseConf.o RtmpServer.o ThreadPool.o User.o $(LIBRARY)

.SUFFIXES: .cpp
.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

clean:
	-rm -f *.o record_server
