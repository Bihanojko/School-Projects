# Subject: Network Applications and Network Administration
# Project: Monitoring of DHCP Communication
# Author: Nikola Valesova
# Login: xvales02
# Date: 19. 11. 2016
# Makefile

CC=g++
CCFLAGS=-std=c++11 -static-libstdc++ -Wextra -Wall -pedantic -pthread

all: dhcp-stats
	
dhcp-stats: dhcp-stats.o get-packets.o process-packets.o output-functions.o
	g++ $(CCFLAGS) -o dhcp-stats output-functions.o process-packets.o get-packets.o dhcp-stats.o -lncurses

dhcp-stats.o: dhcp-stats.cpp
	g++ $(CCFLAGS) -c dhcp-stats.cpp -o dhcp-stats.o

get-packets.o: get-packets.cpp
	g++ $(CCFLAGS) -c get-packets.cpp -o get-packets.o

process-packets.o: process-packets.cpp
	g++ $(CCFLAGS) -c process-packets.cpp -o process-packets.o

output-functions.o: output-functions.cpp
	g++ $(CCFLAGS) -c output-functions.cpp -o output-functions.o

clean:
	rm -f *.o dhcp-stats logfile.csv
