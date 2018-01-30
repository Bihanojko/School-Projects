// Subject: Network Applications and Network Administration
// Project: Monitoring of DHCP Communication
// Author: Nikola Valesova
// Login: xvales02
// Date: 19. 11. 2016
// get-packets.cpp


#include "dhcp-stats.h"


// function that listens to broadcast and filters received packages
int listen_to_broadcast()
{
    int saddr_size, data_size;
    struct sockaddr saddr;
    //unsigned char *buffer = (unsigned char *) malloc(MAX_LENGTH);
    unsigned char buffer[MAX_LENGTH];
	int sock_raw;
    struct iphdr *iph;
    unsigned short iphdrlen;
    struct udphdr *udph;
     
    // try to create socket and check if successfull
    if ((sock_raw = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0)
    {
        cerr << "Error in creating a socket!" << endl;
        return 2;
    }

    while (true)
    {
        // receive packages until received one via UDP protocol containing DHCPv4 communication
        do 
        {
            saddr_size = sizeof saddr;
            data_size = recvfrom(sock_raw, buffer, MAX_LENGTH, 0, &saddr, (socklen_t*)&saddr_size);
            if (data_size < 0)
            {
                cerr << "Error in recvfrom()." << endl;
                return 3;
            }
            // skip ethernet header and get IP header
            iph = (struct iphdr*)(buffer + sizeof(struct ethhdr));
            // get length of ip header            
            iphdrlen = iph->ihl * OCTET_COUNT;
            // get UDP header
            udph = (struct udphdr*)(buffer + iphdrlen  + sizeof(struct ethhdr));
        } while (!(iph->protocol == UDP_PORT && iph->version == 4 &&
        (ntohs(udph->dest) == DEST_PORT_ACK || ntohs(udph->dest) == DEST_PORT_RLS)));

        // count length of all three headers 
        int header_size = sizeof(struct ethhdr) + iphdrlen + sizeof udph;

        // process received packet
        if (find_ip_address(buffer + header_size, data_size - header_size))
            print_output();
    }
    // close socket
    close(sock_raw);
    return 0;
}
