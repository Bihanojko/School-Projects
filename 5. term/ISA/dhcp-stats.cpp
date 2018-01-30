// Subject: Network Applications and Network Administration
// Project: Monitoring of DHCP Communication
// Author: Nikola Valesova
// Login: xvales02
// Date: 19. 11. 2016
// dhcp-stats.cpp

#include "dhcp-stats.h"

// global variable indicating if "-c" parameter was set
bool log_file = false;
// global variable storing number of seconds inserted after "-c" parameter
int interval;
// for locking shared variables among threads
mutex mtx;
// struct containing information about subnets inserted as arguments
struct Subnets_T subnet_list;
// struct containing information about requested IP addresses
struct Req_IP_T req_ip_list;

// MAIN
int main(int argc, char *argv[])
{
    // process and check inserted arguments
	if (parse_args(argc, argv) != 0)
    	return 1;

    // if "-c" parameter was inserted, create a new thread that prints output to logfile every x seconds
    if (log_file)
    {
        thread t1(print_to_log_file);
        t1.detach();
    }

    // start printing output to stdout
    print_output();
    // to check return value of listen_to_broadcast()
    int retval;
    // start listening to broadcast communication
	if ((retval = listen_to_broadcast()) != 0)
        return retval;

    endwin();
	return 0;
}


// function that checks and processes inserted arguments
int parse_args(int argc, char *argv[])
{
    // check argument count
	if (argc < 2)
	{
		cerr << "Missing argument!" << endl;
		return 1;
	}

    // check if help should be printed
    if (argc == 2 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")))
        print_help();

    vector <unsigned int> IP_vec;           // vector storing integer value of octets of IP address
	string argument, IP_address, mask;      // to parse and store inserted arguments: 192.168.0.0/24; 192.168.0.0; 24
	size_t pos;                             // for storing result of find
	signed int int_mask;                    // to store integer value of count of mask bits
	struct sockaddr_in sock_addr;           // struct sockaddr_in for inet_pton function
    bool ok;                                // to sign if current argument is valid 

	for (int i = 1; i < argc; i++)
	{
        IP_vec.clear();
        ok = true;
		argument.assign(argv[i]);

        // if parameter "-c" found, check the following argument if it is only numerical and store the value 
        if (argument == "-c")
        {
            if (i < argc - 1 && argc > 3)
            {
                argument.assign(argv[++i]);
                if (argument.find_first_not_of("0123456789") == string::npos)
                {
                    log_file = true;
                    interval = atoi(argument.c_str());
                }
                else
                    ok = false;
            }
            else
                ok = false;
        }
        // different parameter than "-c", expecting a subnet
        else
        {
            // check if number of bits of prefix is not missing
            pos = argument.find('/');
            if (pos == string::npos || pos == argument.length() - 1)
            {
                cerr << "Missing subnet mask!" << endl;
                return 1;
            }

            // parse subnet into two parts: e.g. 192.168.0.0/24 to 192.168.0.0 and 24
            IP_address = argument.substr(0, pos);
            mask = argument.substr(pos + 1);
            // check if argument contains a valid IP address
            int is_ip = inet_pton(AF_INET, IP_address.c_str(), &(sock_addr.sin_addr));

            // check if mask is only numerical and is between 8 and 30
            if (mask.find_first_not_of("0123456789") == string::npos)
            {
                int_mask = atoi(mask.c_str());
                if (int_mask < MIN_MASK || int_mask > MAX_MASK || !is_ip)
                    ok = false;
            }
            else 
                ok = false;
            
            // if no errors have been found and all values have been stored 
            if (ok == true)
            {
                unsigned int octet;                 // to store one octet of IP address
                unsigned int tmp_mask = int_mask;   // to store value of remaining bits of mask
                string tmp_IP = IP_address;         // to store remaining octets of IP address

                // find next octet
                do
                {
                    pos = tmp_IP.find('.');
                    octet = atoi(tmp_IP.substr(0, pos).c_str());
                    tmp_IP = tmp_IP.substr(pos + 1);

                    // if number of remaining bits of mask is greater than 8, decrement it
                    // if not, null all the remaining bits to get correct IP address of subnet
                    if (tmp_mask < BITS_IN_OCTET)
                    {
                        octet = octet & (ALL_ONES << (BITS_IN_OCTET - tmp_mask));
                        tmp_mask = 0;
                    }
                    else
                        tmp_mask -= BITS_IN_OCTET;
                    IP_vec.push_back(octet);
                } while (pos != string::npos);
            }
        }

        // if error has been found, print error message and return
        if (ok == false)
        {
 			cerr << "Invalid argument: " << argument << "!" << endl;
			return 1;
        }

        // if previous parameter was not "-c", store information about inserted subnet
        if (strcmp(argv[i - 1], "-c"))
        {
            subnet_list.IP_address.push_back(IP_vec);
            subnet_list.mask.push_back(int_mask);
            if (int_mask == 32)
                subnet_list.max_hosts.push_back(1);
            else if (int_mask == 31)
                subnet_list.max_hosts.push_back(2);
            else
                subnet_list.max_hosts.push_back((1 << ((OCTET_COUNT * BITS_IN_OCTET) - int_mask)) - 2);
            subnet_list.current_hosts.push_back(0);
        }
	}

	return 0;
}


// function that waits until end of IPs lease time and then erases the IP address 
void wait_till_leasetime(vector <unsigned int> req_IP_address, unsigned int lease_time)
{
    // find relevant IP address and store this threads ID 
    mtx.lock();
    for (vector <int>::size_type i = 0; i != req_ip_list.IP_address.size(); i++)
    {
        if (req_IP_address == req_ip_list.IP_address[i])
            req_ip_list.thread_ids[i] = this_thread::get_id();
    }
    mtx.unlock();
    
    // wait until end of lease time
    sleep(lease_time);
 
    // find relevant IP address again and check if it has not been renewed
    mtx.lock();
    for (vector <int>::size_type i = 0; i != req_ip_list.IP_address.size(); i++)
    {
        if (req_IP_address == req_ip_list.IP_address[i] && 
        req_ip_list.thread_ids[i] == this_thread::get_id())
        {
            // if it has not been renewed, remove the IP address from list
            remove_record(req_IP_address);
            mtx.unlock();
            return;
        }
    }
    mtx.unlock();
    return;
}