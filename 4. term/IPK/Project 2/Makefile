# Subject: Computer Communications and Networks
# Project: Project 2 - File transfer
# Author: Nikola Valesova
# Login: xvales02
# Date: 24. 4. 2016
# Makefile

CC=g++
CFLAGS=-std=c++11 -static-libstdc++ -Wextra -Wall -pedantic

all: client server

client.o: client.cpp
	$(CC) $(CFLAGS) -c client.cpp -o client.o

server.o: server.cpp
	$(CC) $(CFLAGS) -c server.cpp -o server.o

client: client.o
	$(CC) $(CFLAGS) client.o -o client
	
server: server.o
	$(CC) $(CFLAGS) -pthread server.o -o server

clean:
	rm *.o client server
