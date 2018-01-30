// Subject: Network Applications and Network Administration
// Project: Monitoring of DHCP Communication
// Author: Nikola Valesova
// Login: xvales02
// Date: 19. 11. 2016
// process-packets.cpp


#include "dhcp-stats.h"


// function that goes through a received packet and gets information out of it
int find_ip_address(unsigned char *data, int size)
{
    vector <unsigned int> req_IP_address;   // to store IP address found in received packet
    bool is_release = false;                // to indicate that this message is a valid release
    bool is_ack = false;                    // to indicate that this message is a valid acknowledge
    bool lt_set = false;                    // to indicate that lease time is set
    int IP_address_start;                   // position of first bit of client's IP address
    unsigned int lease_time = 0;            // to count and store value of lease time 

    // go through options
    for (int i = BOOTP_HEADER_LENGTH; i < size; i++)
    {
        unsigned int a = data[i];

        // option DHCP message type: Acknowledge (5)
        if (a == MSGTYPE_OPTION_NUMBER && i < size - 2 && (unsigned int)data[i + 2] == 5)
        {
            is_ack = true;
            i += 2;
            IP_address_start = ACK_CLIENT_ADDR;
        }

        // option DHCP message type: Release (7)
        else if (a == MSGTYPE_OPTION_NUMBER && i < size - 2 && (unsigned int)data[i + 2] == 7)
        {
            is_release = true;
            i += 2;
            IP_address_start = RLS_CLIENT_ADDR;
        }

        // option DHCP message type different from ACK and Release
        else if (a == MSGTYPE_OPTION_NUMBER && !(is_release || is_ack))
            return 0;

        // option lease time
        else if (a == LT_OPTION_NUMBER && i < size - 5)
        {
            lt_set = true;
            int length = (unsigned int)data[i + 1];
            // convert lease time from multiple hexadecimal values to one decimal
            for (int j = 0; j < length; j++)
            {
                lease_time *= MULTIPLY;  // 16 * 16
                lease_time += (unsigned int)data[i + j + 2];
            }
        }

        // if there is an option starting at current position, move option length forward
        else if (a != 0 && i < size - 1)
            i += (unsigned int)data[i + 1] + 1;

        // if message is a valid acknowledge and lease time has been set 
        if (is_ack && lt_set)
        {
            // get destination IP address
            for (int j = 0; j < OCTET_COUNT; j++)
                req_IP_address.push_back(data[IP_address_start + j]);
            
            // if this address hasn't been added to the list yet, add all information about it and create a new thread
            mtx.lock();
            if (new_ip_address(req_IP_address, lease_time))
            {
                req_ip_list.IP_address.push_back(req_IP_address);
                req_ip_list.lease_time.push_back(lease_time);
                req_ip_list.thread_ids.push_back(thread::id(0));
                req_ip_list.threads.push_back(thread(wait_till_leasetime, req_IP_address, lease_time));
                req_ip_list.threads.back().detach();
                // check subnets and increment current appliances
                check_subnets(req_IP_address, 1);
            }
            mtx.unlock();
            return 1;
        }
        // if message is a valid release
        else if (is_release)
        {
            // get source IP address
            for (int j = 0; j < OCTET_COUNT; j++)
                req_IP_address.push_back(data[IP_address_start + j]);
            // remove this IP address from the list and decrement current appliances
            remove_record(req_IP_address);
            return 1;
        }
    }
    return 0;
}


// function that finds out if IP address is already in the list
bool new_ip_address(vector <unsigned int> req_IP_address, unsigned int lease_time)
{
    // try to find the IP address
    for (vector <int>::size_type i = 0; i != req_ip_list.IP_address.size(); i++)
    {
        // if found, store new lease time value and create new thread to wait until the new lease time
        if (req_IP_address == req_ip_list.IP_address[i])
        {
            req_ip_list.lease_time[i] = lease_time;
            req_ip_list.threads.push_back(thread(wait_till_leasetime, req_IP_address, lease_time));
            req_ip_list.threads.back().detach();             
            return false;
        }
    }
    return true;
}


// function that returns true when IP address is brodcast
bool is_broadcast(vector <unsigned int> req_IP_address, int mask)
{
    int octet_num = 0;

    for (int i = 0; i < OCTET_COUNT; i++)
    {
        if (mask >= BITS_IN_OCTET)
            mask -= BITS_IN_OCTET;
        // if there are less than 8 bits of mask remaining, start checking the remaining bits of IP address
        else 
        {
            unsigned char broadcast = req_IP_address[octet_num] | (~(ALL_ONES << (BITS_IN_OCTET - mask)));
            if (req_IP_address[octet_num] != broadcast)
                return false;
            mask = 0;
        }
        octet_num++;
    }
    return true;
}


// function that finds subnets in which IP address belongs and increments or decrements
// the count of current appliances in it (depending on operand)
void check_subnets(vector <unsigned int> req_IP_address, signed int operand)
{
    int mask_tmp;               // number of remaining bits of mask 
    unsigned int octet_num;     // number of octet of IP address

    for (vector <int>::size_type i = 0; i != subnet_list.IP_address.size(); i++)
    {
        mask_tmp = subnet_list.mask[i];
        octet_num = 0;

        // do not count IP addresses equal to subnet and broadcast
        if ((subnet_list.mask[i] < 31 && req_IP_address != subnet_list.IP_address[i] && !is_broadcast(req_IP_address, mask_tmp))
        || (subnet_list.mask[i] > 30))
        {
            while (true)
            {
                // if there are more than 8 bits of mask remaining, check if (octet_num)th octets have the same value
                if (mask_tmp > BITS_IN_OCTET)
                {
                    if (subnet_list.IP_address[i][octet_num] != req_IP_address[octet_num])
                        break;
                    mask_tmp -= BITS_IN_OCTET;
                }
                // if there are less than 8 bits of mask remaining, check only leftmost mask_tmp bits, if they are the same
                else
                {
                    if (~subnet_list.IP_address[i][octet_num] != ~(req_IP_address[octet_num] & (ALL_ONES << (BITS_IN_OCTET - mask_tmp))))
                        break;
                    subnet_list.current_hosts[i] += operand;
                    break;
                }
                octet_num++;
            }
        }
    } 
}


// function that removes information about one IP address
void remove_record(vector <unsigned int> req_IP_address)
{
    // find the IP address in the list of all IP addresses
    for (vector <int>::size_type i = 0; i != req_ip_list.IP_address.size(); i++)
    {
        for (vector <int>::size_type j = 0; j != req_ip_list.IP_address[i].size(); j++)
        {
            if (req_ip_list.IP_address[i][j] != req_IP_address[j])
                break;
            // if found, decrement number of current appliances in corresponding subnets
            // and remove the record about this IP address
            if (j == req_ip_list.IP_address[i].size() - 1)
            {
                check_subnets(req_IP_address, -1);
                req_ip_list.IP_address.erase(req_ip_list.IP_address.begin() + i);
                req_ip_list.lease_time.erase(req_ip_list.lease_time.begin() + i);
                req_ip_list.threads.erase(req_ip_list.threads.begin() + i);
                req_ip_list.thread_ids.erase(req_ip_list.thread_ids.begin() + i);
                print_output();
                return;
            }
        }
    }
}