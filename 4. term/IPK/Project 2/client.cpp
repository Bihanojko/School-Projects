// Subject: Computer Communications and Networks
// Project: Project 2 - File transfer
// Author: Nikola Valesova
// Login: xvales02
// Date: 24. 4. 2016
// Client


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <cstdlib>
#include <algorithm>
#include <unistd.h>
#include <fstream>
#include <cstdio>
#include <sstream>


const int BUFSIZE = 512;


// to sort out problem with standard to_string function
template <typename T>
std::string to_string(T value)
{
	std::ostringstream os ;
	os << value ;
	return os.str() ;
}

// procedure that writes out an error message to stderr
// @param: string containing the error message
void print_error(std::string err_msg) {
	std::cerr << err_msg << std::endl;
}

// function to parse inserted arguments and check if they are valid
// @param: argc - argument count
// @param: argv - argument vector
// @param: hostname - name of the required host
// @param: port - port number
// @param: download - required operation - true if download
// @param: filename - name of the required file
// @param: path_to_file - path to the required file
int parse_args(int argc, char **argv, std::string &hostname, int &port, 
bool &download, std::string &filename, std::string &path_to_file) {
	if (argc != 7) {
		print_error("ERROR: Invalid number of arguments!");
		return 1;
	}

	int c;
	size_t pos;
	bool h = false, p = false, down = false;
	while ((c = getopt(argc, argv, "h:p:u:d:")) != -1) {
		std::istringstream ss(optarg);
		switch (c) {
			case 'h':
				if (h) {
					print_error("ERROR: Invalid arguments!");
					return 1;					
				}
				h = true;
				hostname = optarg;
				break;
			case 'p':
				if (p) {
					print_error("ERROR: Invalid arguments!");
					return 1;					
				}
				p = true;
				if (!(ss >> port)) {
					print_error("ERROR: Invalid port number!");
					return 1;
				}
				break;
			case 'u':
				if (down) {
					print_error("ERROR: Invalid arguments!");
					return 1;					
				}
				down = true;
				download = false;
				path_to_file = optarg;
				pos = path_to_file.find_last_of("/");
				if (pos == std::string::npos)
					filename = optarg;
				else if (pos == path_to_file.length() - 1) {
					print_error("ERROR: Choose a file to upload, not a folder!");
					return 1;
				}
				else
					filename = path_to_file.substr(pos + 1);
				break;
			case 'd':
				if (down) {
					print_error("ERROR: Invalid arguments!");
					return 1;					
				}
				down = true;
				download = true;
				filename = optarg;
				break;
			case '?':
			default:
				print_error("ERROR: Invalid arguments!");
				return 1;
				break;
		}
	}

	return 0;
}

// function to establish a connection with the server
// @param: number of the created socket
int create_socket(int &client_socket) {
	if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) <= 0) {
		print_error("ERROR in socket!");
		return 1;
	}
	return 0;
}

// function to get host by name
int getme_host(struct hostent * &server, std::string hostname) {
	if ((server = gethostbyname(hostname.c_str())) == NULL) {
		print_error("ERROR in translating a domain into an IPv4 address!");
		return 1;
	}
	return 0;
}

// function to clear memory and connect to the server
int set_n_connect(struct hostent * &server, struct sockaddr_in &server_address, int port, int client_socket) {
	memset((void *)&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&server_address.sin_addr.s_addr, server->h_length);
	server_address.sin_port = htons(port);

	if (connect(client_socket, (const struct sockaddr *)&server_address, sizeof(server_address)) != 0) {
		close(client_socket);
		print_error("ERROR: connect!");
		return 1;
	}
	return 0;
}

// function that sends request or other bytes of information to server
int send_request(int client_socket, std::string request) {
	int bytestx = send(client_socket, request.c_str(), request.size(), 0);
	if (bytestx < 0) {
		print_error("ERROR in send!");
		close(client_socket);
		return 1;
	}
	return 0;
}

// function that receives data until a whole header is received 
int get_header(int client_socket, std::string &head) {
	int bytestx;
	char buf[BUFSIZE + 1];
	size_t pos;
	head = "";

	while (true)
	{
		bytestx = recv(client_socket, buf, BUFSIZE, 0);
		if (bytestx < 0) {
			print_error("ERROR: recv!");
			close(client_socket);
			return 1;
		}
		head.append(buf, bytestx);
		if ((pos = head.find("\r\n\r\n")) != std::string::npos)
			break;
	}
	return 0;
}

// function that checks the received header
int check_response(std::string &received_data, int client_socket) {
	std::string response = received_data.substr(0, 6);

	if (response == "100 OK") {
		int bytestx;
		char buf[BUFSIZE + 1];
		while ((bytestx = recv(client_socket, buf, BUFSIZE, 0)) != 0)
		{
			if (bytestx < 0) {
				print_error("ERROR: recv!");
				close(client_socket);
				return 2;
			}
			received_data.append(buf, bytestx);
		}
		return 0;
	} else if (response == "102 EF") {
		print_error("ERROR: Demanded file does not exist or is unreadable!");
		close(client_socket);
		return 3;
	} else {
		close(client_socket);
		return 2;
	}
}

// function that skips header and writes out the received data to an output file
int write_out(std::string received_data, std::string filename) {
	size_t pos;
	while ((pos = received_data.find("\r\n\r\n")) == std::string::npos)
		;

	std::string body = received_data.substr(pos + 4);

	std::fstream fw;
	fw.open(filename.c_str(), std::ios::out | std::ios::binary);
	if (!fw.is_open()) {
		print_error("ERROR: creating or opening file!");
		return 1;
	}

	fw << body;
	fw.close();

	return 0;
}

// function that sends header and body containing the required file
int send_data(int client_socket, std::string path_to_file) {
	std::ifstream fr;
	fr.open(path_to_file, std::ifstream::ate | std::ifstream::binary);
	if (!fr.is_open()) {
		print_error("ERROR: creating or opening file!");
		std::string message = "102 EF\r\n\r\n";
		send_request(client_socket, message);
		close(client_socket);
		return 1;
	}

	fr.seekg(0, fr.end);
	int data_length = fr.tellg();
	fr.seekg(0, fr.beg);
	std::string add_head = "100 OK\r\n";
	add_head.append(to_string(data_length));
	add_head.append("\r\n\r\n");

	send_request(client_socket, add_head);
	char buf[BUFSIZE + 1];
	int read_data = 0;

	while (true) {
		fr.read(buf, BUFSIZE);
		read_data += fr.gcount();
		if ((send(client_socket, buf, fr.gcount(), 0)) < 0) {
			print_error("ERROR in send!");
			break;
		}

		if (data_length == read_data)
			break;
	}

	fr.close();
	return 0;
}


// MAIN
int main(int argc, char **argv) {
	using namespace std;

	string hostname, filename, path_to_file, request, received_data;
	int port;
	int client_socket;
	struct hostent *server;
	struct sockaddr_in server_address;
	bool download;

	if (parse_args(argc, argv, hostname, port, download, filename, path_to_file))
		return 1;

	if (getme_host(server, hostname))
		return 2;

	if (create_socket(client_socket))
		return 3;

	if (set_n_connect(server, server_address, port, client_socket))
		return 4;

	if (download) {
		request = "TCP\r\nDownload request: " + filename + "\r\n\r\n";
		if (send_request(client_socket, request))
			return 5;
		if (get_header(client_socket, received_data))
			return 6;
		if (check_response(received_data, client_socket))
			return 7;
		if (write_out(received_data, filename)) {
			close(client_socket);
			return 8;
		}
	} else {
		request = "TCP\r\nUpload request: " + filename + "\r\n\r\n";
		if (send_request(client_socket, request))
			return 5;

		if (send_data(client_socket, path_to_file))
			return 6;
	}

	close(client_socket);
	return 0;
}
