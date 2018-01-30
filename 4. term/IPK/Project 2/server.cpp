// Subject: Computer Communications and Networks
// Project: Project 2 - File transfer
// Author: Nikola Valesova
// Login: xvales02
// Date: 24. 4. 2016
// Server


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
#include <thread>


// size of buffer for reading or receiving data
const int BUFSIZE = 512;
// maximum number of requests at a time
const int max_backlog = 10;



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


// function to check and process arguments
// @param: argc - argument count
// @param: argv - argument vector
// @param: port - port number set as an argument
int parse_args(int argc, char **argv, int &port) {
	if (argc != 3 || (strcmp(argv[1], "-p") != 0)) {
		print_error("ERROR: Invalid arguments, run as: server -p <portnumber>!");
		return 1;
	}

	std::istringstream ss(argv[2]);
	if (!(ss >> port)) {
		print_error("ERROR: Invalid port number!");
		return 1;
	}

	return 0;
}


// function to create a socket
// @param: number of the created socket
int create_socket(int &server_socket) {
	if ((server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP)) <= 0) {
		print_error("ERROR in creating socket!");
		return 1;
	}
	return 0;
}


// function to bind the server and the created socket
// @param: server - sockadd_in structure
// @param: server_socket - number of the created socket
// @param: port - port number
int set_n_bind(struct sockaddr_in &server, int server_socket, int port) {
	bzero((char *) &server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);

	if ((bind(server_socket, (struct sockaddr*)&server, sizeof(server))) < 0) {
		close(server_socket);
		print_error("ERROR: bind!");
		return 1;
	}
	return 0;
}


// function to accept a client's request
// @param: server_socket - number of the created socket
// @param: sa_client - sockaddr_in structure
// @param: comm_socket - number of the client's socket
int accept_request(int server_socket, struct sockaddr_in &sa_client, int &comm_socket) {
	bzero((char *) &sa_client, sizeof(sa_client));
	socklen_t sa_client_len = sizeof(sa_client);
	comm_socket = accept(server_socket, (struct sockaddr*)&sa_client, &sa_client_len);
	if (comm_socket < 1) {
		close(server_socket);
		print_error("ERROR: accept!");
		return 1;		
	}

	return 0;
}

// function that receives data until whole header is received 
int get_header(int comm_socket, std::string &head) {
	int bytestx;
	char buf[BUFSIZE + 1];
	size_t pos;
	head = "";

	while (true) {
		bytestx = recv(comm_socket, buf, BUFSIZE, 0);
		if (bytestx < 0) {
			print_error("ERROR: recv!");
			return 1;
		}
		head.append(buf, bytestx);
		if ((pos = head.find("\r\n\r\n")) != std::string::npos)
			break;
	}
	return 0;
}

// function to write out received data to output file
void write_out(std::string received_data, std::string output_filename) {
	std::fstream fw;
	fw.open(output_filename.c_str(),  std::ios::out | std::ios::binary);
	if (!fw.is_open()) {
		print_error("ERROR: creating or opening file!");
		return;
	}
	fw << received_data;
	fw.close();
	return;
}

// function that receives data, checks header and reads the whole body 
void get_data(int comm_socket, std::string filename, std::string &body) {
	int bytestx;
	char buf[BUFSIZE + 1];
	size_t pos;
	int downloaded = body.length();
	int data_length = -1;
	bool found = false;

	while (true) {	
		bytestx = recv(comm_socket, buf, BUFSIZE, 0);
		if (bytestx < 0) {
			print_error("ERROR: recv!");
			return;
		}
		body.append(buf, bytestx);
		downloaded += bytestx;

		if (body.compare(0, 6, "100 OK") == 0) {
			body = body.substr(8);
			downloaded -= 8;
		} else if (body.compare(0, 6, "102 EF") == 0)
			return;

		if (!found) {
			if ((pos = body.find("\r\n\r\n")) != std::string::npos) {
				std::istringstream ss(body.substr(0, pos));
				if (!(ss >> data_length)) {
					print_error("ERROR: Invalid size of file, header corrupted!");
					return;
				} else {
					found = true;
					body = body.substr(pos + 4);
					downloaded -= pos + 4;
				}
			}
		}

		if (downloaded == data_length)
			break;
		else if (downloaded > data_length) {
			print_error("ERROR: Received file may be corrupted!");
			break;
		}
	}

	write_out(body, filename);
	return;
}

// function that opens required file and sends the answering message
void send_data(int comm_socket, std::string filename) {
	std::ifstream fr;
	fr.open(filename, std::ifstream::ate | std::ifstream::binary);
	
	if (!fr.is_open()) {
		print_error("ERROR: creating or opening file!");
		std::string message = "102 EF\r\n\r\n";

		int bytestx = send(comm_socket, message.c_str(), message.size(), 0);
		if (bytestx < 0) {
			print_error("ERROR in send!");
			return;
		}
		return;
	}

	fr.seekg(0, fr.end);
	int data_length = fr.tellg();
	fr.seekg(0, fr.beg);

	std::string add_head = "100 OK\r\n";
	add_head.append(to_string(data_length));
	add_head.append("\r\n\r\n");

	int bytestx = send(comm_socket, add_head.c_str(), add_head.size(), 0);
	if (bytestx < 0) {
		print_error("ERROR in send!");
		fr.close();
		return;
	}
	
	char buf[BUFSIZE + 1];
	int read_data = 0;
	while(true) {
		fr.read(buf, BUFSIZE);
		read_data += fr.gcount();
		bytestx = send(comm_socket, buf, fr.gcount(), 0);
		if (bytestx < 0) {
			print_error("ERROR in send!");
			fr.close();
			return;
		}

		if (read_data == data_length)
			break;
	}

	fr.close();
	return;
}

// function that checks the received header and gets a type of operation
void process_request(int &comm_socket) {
	std::string head;

	if (get_header(comm_socket, head)) {
		close(comm_socket);
		return;
	}

	if (head.compare(5, 17, "Download request:") == 0) {
		size_t pos = head.find("\r\n\r\n");
		std::string request = "TCP\r\nDownload request: ";
		size_t len = request.length();
		std::string filename = head.substr(len, pos - len);
		send_data(comm_socket, filename);
	} else if (head.compare(5, 15, "Upload request:") == 0) {
		size_t pos = head.find("\r\n\r\n");
		std::string tmp = "TCP\r\nUpload request: ";
		size_t len = tmp.length();
		std::string filename = head.substr(len, pos - len);
		tmp = "\r\n\r\n";
		pos += tmp.length();
		tmp = head.substr(pos);
		get_data(comm_socket, filename, tmp);
	} else {
		print_error("ERROR: header corrupted!");
	}

	close(comm_socket);
	return;
}


// MAIN
int main(int argc, char **argv) {
	int server_socket, comm_socket, port;
	struct sockaddr_in server, sa_client; 

	if (parse_args(argc, argv, port))
		return 5;

	if (create_socket(server_socket) ||
		set_n_bind(server, server_socket, port))
		return 1;

	if ((listen(server_socket, max_backlog)) < 0) {
		close(server_socket);
		print_error("ERROR: listen!");
		return 2;		
	}

	while(true) {
		if (accept_request(server_socket, sa_client, comm_socket))
			return 3;
		std::thread(process_request, std::ref(comm_socket)).detach();
	}

	close(server_socket);
	return 0;
}