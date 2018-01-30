// Subject: Network Applications and Network Administration
// Project: Monitoring of DHCP Communication
// Author: Nikola Valesova
// Login: xvales02
// Date: 19. 11. 2016
// dhcp-stats.h


#ifndef DHCPSTATS_H
#define DHCPSTATS_H


// HEADERS

#include <string.h>
#include <iostream>
#include <arpa/inet.h>
#include <vector>
#include <unistd.h>
#include <netinet/udp.h>
#include <netinet/ip.h>
#include <net/ethernet.h>
#include <sys/socket.h>
#include <math.h>
#include <ncurses.h>
#include <thread>
#include <time.h>
#include <mutex>


using namespace std;


// CONSTANTS

// destination port of DHCP acknowledge
const int DEST_PORT_ACK = 68;
// destination port of DHCP release
const int DEST_PORT_RLS = 67;
// port of UDP protocol
const int UDP_PORT = 17;
// number of octets in one IPv4 address
const int OCTET_COUNT = 4;
// all ones in one char, maximum value of char, for bit operations
const unsigned int ALL_ONES = 255;
// number of bits in one octet, one char
const int BITS_IN_OCTET = 8;
// minimum number of bits of prefix
const int MIN_MASK = 1;
// maximum number of bits of prefix
const int MAX_MASK = 32;
// maximum length of a packet, including all options and all headers
const int MAX_LENGTH = 65536;
// fixed length of a BOOTP header
const int BOOTP_HEADER_LENGTH = 240;
// position of the first bit of destination address in ACK 
const int ACK_CLIENT_ADDR = 16;
// position of the first bit of source address in Release
const int RLS_CLIENT_ADDR = 12;
// option number indicating lease time
const int LT_OPTION_NUMBER = 51;
// option number indicating type of message
const int MSGTYPE_OPTION_NUMBER = 53;
// number to be multipled to get lease time in decadic format (16 * 16)
const int MULTIPLY = 256;


// GLOBAL VARIABLES

// global variable indicating if "-c" parameter was set
extern bool log_file;
// global variable storing number of seconds inserted after "-c" parameter
extern int interval;
// mutex for locking shared variables among threads
extern mutex mtx;

// struct containing information about subnets inserted as arguments 
extern struct Subnets_T {
    vector <vector <unsigned int> > IP_address;
    vector <int> mask;
    vector <int> max_hosts;
    vector <int> current_hosts;
} subnet_list;

// struct containing information about requested IP addresses
extern struct Req_IP_T {
    vector <vector <unsigned int> > IP_address;
    vector <unsigned int> lease_time;
    vector <thread> threads;
    vector <thread::id> thread_ids;
} req_ip_list;


// FUNCTION PROTOTYPES

int parse_args(int argc, char *argv[]);
int find_ip_address(unsigned char* data , int Size);
void wait_till_leasetime(vector <unsigned int> req_IP_address, unsigned int lease_time);
bool new_ip_address(vector <unsigned int> req_IP_address, unsigned int lease_time);
void check_subnets(vector <unsigned int> req_IP_address, signed int operand);
void remove_record(vector <unsigned int> req_IP_address);
int listen_to_broadcast();
int print_to_log_file();
void print_output();
void print_help();
bool is_broadcast(vector <unsigned int> req_IP_address, int mask);

#endif
