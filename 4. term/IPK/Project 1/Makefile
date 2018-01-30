NAME=webclient
CC=g++
CFLAGS=-std=c++11 -static-libstdc++ -Wextra -Wall -pedantic

all: webclient

$(NAME): webclient.o
	g++ $(CFLAGS) webclient.o -o webclient
	
webclient.o: webclient.cpp
	g++ $(CFLAGS) -c webclient.cpp

clean:
	rm *.o *.html webclient
