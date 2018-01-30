// Subject: Computer Communications and Networks
// Project: Project 1 - Webclient
// Author: Nikola Valesova
// Login: xvales02
// Date: 10. 3. 2016


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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


const int BUFSIZE = 512;
bool parse = true;


// procedure that writes out an error message to stderr
// @param: string containing the error message
void print_error(std::string err_msg) {
	std::cerr << err_msg << std::endl;
}


// function to get domain name, filename and file path from url
// @params: arg - whole url, url_part1 - domain name, url_part2 - file path
// @params: output_filename - name of the file to be written to, port - port number 
int parse_url(std::string &arg, std::string &url_part1, std::string &url_part2, std::string &output_filename, long &port) {
	using namespace std;

	if (arg.find("http://") == 0)
		arg.erase(0, 7);
	else if (arg.find("https://") == 0) {
		print_error("Invalid URL, not supporting https websites!");
		return 1;
	}

	size_t pos = arg.find(".", 0);
	if (pos == string::npos) {
		print_error("Invalid URL!");
		return 1;
	}

	pos = arg.find("/");
	if (pos != string::npos) {
		url_part1 = arg.substr(0, pos);
		url_part2 = arg.substr(pos);
	} else {
		url_part1 = arg;
		url_part2 = "/";		
	}

	pos = url_part1.find(":");
	if (pos != string::npos) {
		char *endptr;
		string port_str = url_part1.substr(pos + 1);
		port = strtol(port_str.c_str(), &endptr, 10);
		if (*endptr != '\0' || endptr == port_str) {
			print_error("Invalid URL, port number expected!");
			return 1;
		}
		url_part1 = url_part1.substr(0, pos);
	} else
		port = 80;

	if (parse == true) {
		pos = arg.find_last_of("/");
		if (pos != string::npos) {
			output_filename = arg.substr(pos);

			if (output_filename == "/")
				output_filename = "index.html";
			else {
				output_filename = output_filename.substr(1);
				if ((pos = output_filename.find("?")) != string::npos)
					output_filename = output_filename.substr(0, pos);
			}
		}
		parse = false;
	}

	pos = 0;
	while ((pos = url_part2.find("%", pos)) != string::npos) {
		url_part2.replace(pos, 1, "%25");
		pos++;
	}
	pos = 0;
	while ((pos = url_part2.find(" ", pos)) != string::npos)
		url_part2.replace(pos, 1, "%20");
	pos = 0;
	while ((pos = url_part2.find("#", pos)) != string::npos)
		url_part2.replace(pos, 1, "%23");
	pos = 0;
	while ((pos = url_part2.find("^", pos)) != string::npos)
		url_part2.replace(pos, 1, "%5E");
	pos = 0;
	while ((pos = url_part2.find(";", pos)) != string::npos)
		url_part2.replace(pos, 1, "%3B");


	return 0;
}


// function to create a socket
// @param: number of the created socket
int create_socket(int &client_socket)
{
	if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) <= 0) {
		print_error("ERROR in socket!");
		return 1;
	}

	return 0;
}


// function that retrieves host information of a host name
// @params: server - hostent structure containing obtained info, url_part1 - domain name
int getme_host(struct hostent * &server, std::string url_part1)
{
	if ((server = gethostbyname(url_part1.c_str())) == NULL) {
		print_error("ERROR, no such host!");
		return 1;
	}

	return 0;
}


// function that creates connection to a server
// @params: server - hostent structure containing server info, server_address - sockaddr structure
// @params: port - port number, client_socket - number of socket
int set_n_connect(struct hostent * &server, struct sockaddr_in &server_address, long port, int client_socket) {
	bzero((char *) &server_address, sizeof(server_address));
	server_address.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&server_address.sin_addr.s_addr, server->h_length);
	server_address.sin_port = htons(port);

	if (connect(client_socket, (const struct sockaddr *)&server_address, sizeof(server_address)) != 0) {
		print_error("ERROR: connect!");
		return 1;
	}

	return 0;
}


// function that sends an http 1.1 request
// @params: client_socket - number of socket, url_part1 - domain name
// @params: url_part2 - file path, arg - whole url
int request_data(int client_socket, std::string url_part1, std::string url_part2, std::string &arg) {
	arg = "GET " + url_part2 + " HTTP/1.1\r\nHost: " + url_part1 + "\r\nConnection: close\r\n\r\n";

	int bytestx = send(client_socket, arg.c_str(), arg.size(), 0);
	if (bytestx < 0) {
		print_error("ERROR in send!");
		return 1;
	}

	return 0;
}


// function that sends an http 1.0 request
// @params: client_socket - number of socket, url_part1 - domain name
// @params: url_part2 - file path, arg - whole url
int request_data0(int client_socket, std::string url_part1, std::string url_part2, std::string &arg) {
	arg = "GET " + url_part2 + " HTTP/1.0\r\nHost: " + url_part1 + "\r\n\r\n";

	int bytestx = send(client_socket, arg.c_str(), arg.size(), 0);
	if (bytestx < 0) {
		print_error("ERROR in send!");
		return 1;
	}

	return 0;
}


// function that receives data until a whole header has arrived
// @params: client_socket - socket number, head - received data containing the header
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
			return 1;
		}
		head.append(buf, bytestx);
		if ((pos = head.find("\r\n\r\n")) != std::string::npos)
			break;
	}
	return 0;
}


// function that checks the response value in the header
// @params: received_data - received data from server, arg - whole url (in case of response 301 or 302)
// @params: client_socket - socket number
int check_response(std::string &received_data, std::string &arg, int client_socket) {
	std::string response = received_data.substr(9, 3);

	if (response == "200") {
		int bytestx;
		char buf[BUFSIZE + 1];
		while ((bytestx = recv(client_socket, buf, BUFSIZE, 0)) != 0)
		{
			if (bytestx < 0) {
				print_error("ERROR: recv!");
				return 2;
			}
			received_data.append(buf, bytestx);
		}
		return 0;
	} else if (response == "301" || response == "302") {
		size_t start_pos = received_data.find("Location: ");
		size_t end_pos = received_data.find("\r\n", start_pos);
		arg = received_data.substr(start_pos + 10, end_pos - (start_pos + 10));
		return 1;
	} else if (response == "505") {
		return 3;
	} else {
		std::cout << "ERROR: server response: " << response << std::endl;
		return 2;
	}
}


// function that parses the rest of the data received from server
// @param: received_data - the whole data received from server
int process_body(std::string &received_data) {
	size_t pos;
	while ((pos = received_data.find("\r\n\r\n")) == std::string::npos)
		;
	std::string header = received_data.substr(0, pos + 3);
	std::string body = received_data.substr(pos + 4);
	std::string body_unchunked = "";
	std::string sub_chunk;
	unsigned int number = 1;
	if (header.find("Transfer-Encoding: chunked") != std::string::npos) {
		while (number != 0) {
			pos = body.find("\r\n");
			sub_chunk = body.substr(0, pos);
			number = strtoul(sub_chunk.c_str(), NULL, 16);
			body_unchunked.append(body.substr(pos + 2, number));
			body.erase(0, pos + number + 4);
		}
		received_data = body_unchunked;
	} else
		received_data = body;

	return 0;
}


// function to write out the data into an output file
// @param: received_data - data obtained from server
// @param: output_filename - name of the file to write to
int write_out(std::string received_data, std::string output_filename) {
	std::fstream fw;
	fw.open(output_filename.c_str(),  std::ios::out | std::ios::binary);
	if (!fw.is_open()) {
		print_error("ERROR: creating or opening file!");
		return 1;
	}
	fw << received_data;
	fw.close();

	return 0;
}


// ********** MAIN **********
int main(int argc, char **argv) {
	using namespace std;

	if (argc != 2) {
		print_error("Invalid arguments!");
		return 1;
	}

	string arg, url_part1, url_part2, output_filename, received_data;
	long port = 80;
	const int MAX_REDIRECTS = 5;
	int redirects = 0;
	arg.assign(argv[1]);
	int client_socket;
	struct hostent *server;
	struct sockaddr_in server_address;

	while (redirects <= MAX_REDIRECTS) {
		received_data.clear();
		output_filename = "index.html";

		if (parse_url(arg, url_part1, url_part2, output_filename, port))
			return 2;

		if (create_socket(client_socket))
			return 3;

		if (getme_host(server, url_part1)) {
			close(client_socket);
			return 4;
		}

		if (set_n_connect(server, server_address, port, client_socket)){
			close(client_socket);
			return 5;
		}

		for (int i = 0; i < 2; i++) {
			if (i == 0) {
				if (request_data(client_socket, url_part1, url_part2, arg)) {
					close(client_socket);
					return 6;
				}
			}
			else {
				if (request_data0(client_socket, url_part1, url_part2, arg)) {
					close(client_socket);
					return 6;				
				}
			}

			if (get_header(client_socket, received_data)) {
				close(client_socket);
				return 7;
			}

			int ret_val = check_response(received_data, arg, client_socket);
			if (ret_val == 2) {
				close(client_socket);
				return 8;
			}
			else if (ret_val == 0) {
				if (process_body(received_data)) {
					close(client_socket);
					return 9;
				}
				if (write_out(received_data, output_filename)) {
					close(client_socket);
					return 10;
				}
				redirects = 100;
			} 
			if (ret_val != 3)
				break;
		}

		close(client_socket);
		redirects++;
	}

	if (redirects == 6) {
		print_error("ERROR: Maximum number of redirects exceeded!");
		return 11;
	}
	return 0;
}