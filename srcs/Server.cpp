/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adesille <adesille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 11:06:23 by adesille          #+#    #+#             */
/*   Updated: 2025/04/24 10:05:50 by adesille         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "Server.hpp"

Server::Server(int port, const std::string& password) : port(port), password(password) { setupSocket(); }
Server::~Server() { };

/*
*	This function initializes the server's networking capabilities (creating like a mailbox):
*	1. Creates a socket for network communication
*	2. Configures socket options for reuse and non-blocking mode
*	3. Sets up server address (IPv4, any available interface, specified port)
*	4. Binds the socket to the address
*	5. Starts listening for incoming connections
*	6. Adds the server socket to poll monitoring
*/
void	Server::setupSocket() {
	// Create an IPv4 (AdressFamily_InterNET) TCP (SOCKet_STREAM) socket
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket < 0) {
		perror("socket");
		exit(1);
	}
	int opt = 1;
	// SO_REUSEADDR allows the socket to be bound to an address that's in TIME_WAIT state
	// This prevents "Address already in use" errors when restarting the server
	setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	// Set socket to non-blocking mode - operations won't wait for completion
	// This allows the server to handle multiple clients without blocking
	fcntl(serverSocket, F_SETFL, O_NONBLOCK);

	// Initialize server address structure
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;          // IPv4 protocol
	serverAddr.sin_addr.s_addr = INADDR_ANY;  // Accept connections on any interface
	serverAddr.sin_port = htons(port);        // Convert port to network byte order

	// Bind the socket to the configured address and port
	if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
		perror("bind");
		exit(1);
	}

	// Start listening for incoming connections with a backlog queue of 5
	listen(serverSocket, 5);

	// Add the server socket to the pollfd vector for monitoring incoming connections
	struct pollfd pfd;
	pfd.fd = serverSocket;
	pfd.events = POLLIN;  // Monitor for incoming data (connection requests)
	pollfds.push_back(pfd);
}

/*
*	This function handles new client connections to the server:
*	1. Accepts the new connection from a client
*	2. Sets the new connection to non-blocking mode
*	3. Creates a client object and adds it to the clients map
*	4. Sets up polling for the new client socket
*	-> Each client gets his own unique socket (like a dedicated phone line)
*/
void Server::handleNewConnection() {
	
	struct sockaddr_in clientAddr;
	socklen_t clientLen = sizeof(clientAddr);
	// Accept returns a new file descriptor for the client connection
	int clientFd = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);

	if (clientFd < 0) {
		perror("accept");
		return;
	}

	// Set client socket to non-blocking mode
	fcntl(clientFd, F_SETFL, O_NONBLOCK);

	// Create a new Client object and add it to the clients map using the fd as key
	Client* newClient = new Client(clientFd);
	clients[clientFd] = newClient;

	// Add the new client socket to the poll monitoring list
	struct pollfd pfd;
	pfd.fd = clientFd;
	pfd.events = POLLIN;  // Monitor for incoming data from this client
	pollfds.push_back(pfd);

	std::cout << "New client connected: " << clientFd << std::endl;
}

/*
*	Main server loop:
*	1. Uses poll() to monitor all sockets for activity
*	2. Handles new connections when the server socket is active
*	3. Processes data from existing clients when their sockets are active
*/
void Server::run() {
	
	while (true) {
		// poll() monitors multiple file descriptors, waiting for one to become ready
		// -1 timeout means wait indefinitely until an event occurs
		int ret = poll(&pollfds[0], pollfds.size(), -1);
		if (ret < 0) {
			perror("poll");
			continue;
		}

		// Check if the server socket has activity (index 0 in pollfds) - this means a new connection
		if (pollfds[0].revents & POLLIN) {
			handleNewConnection();
		}

		// Check all client sockets for activity
		// Starting from index 1 because index 0 is the server socket
		for (size_t i = 1; i < pollfds.size(); ++i) {
			if (pollfds[i].revents & POLLIN) {
				// POLLIN indicates there's data to read from this client
				handleClientData(i);
			}
		}
	}
}

/*
*	Process incoming data from a specific client:
*	1. Reads data from the client socket
*	2. Appends data to the client's buffer
*	3. Processes complete messages (delimited by \r\n)
*/
void Server::handleClientData(int index) {
	
	int fd = pollfds[index].fd;
	Client* client = clients[fd];
	// IRC protocol limits messages to 512 bytes including CRLF
	char buffer[512];
	// recv() reads data from socket into buffer
	ssize_t bytesRead = recv(fd, buffer, sizeof(buffer), 0);

	if (bytesRead <= 0) {
		// Client disconnected or error occurred
		removeClient(fd);
		return;
	}

	// Add received data to client's buffer
	client->buffer.append(buffer, bytesRead);

	// IRC messages are terminated by \r\n
	// Process complete messages while leaving partial messages in buffer
	size_t pos;
	while ((pos = client->buffer.find("\r\n")) != std::string::npos) {
		std::string message = client->buffer.substr(0, pos);
		client->buffer.erase(0, pos + 2);  // Remove processed message including \r\n
		processCommand(client, message);
	}
}

void Server::processCommand(Client* client, const std::string& message) {
	/*
	{
    	// Command parsing and processing logic here
	}
	*/
    std::cout << "Received: " << message << std::endl;
    client->sendReply(":" + client->nickname + "!" + client->username + "@localhost " + message + "\r\n");
}

void Server::removeClient(int fd) {
    std::map<int, Client*>::iterator it = clients.find(fd);
    if (it != clients.end()) {
        delete it->second;
        clients.erase(it);
    }

    for (size_t i = 0; i < pollfds.size(); ++i) {
        if (pollfds[i].fd == fd) {
            pollfds.erase(pollfds.begin() + i);
            break;
        }
    }

    close(fd);
}

void Server::broadcastMessage(const std::string& message, Client* sender, Channel* channel) {
    if (channel) {
        for (std::map<int, Client*>::iterator it = channel->members.begin(); it != channel->members.end(); ++it) {
            if (it->second != sender) {
                it->second->sendReply(message);
            }
        }
    } else {
        for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
            if (it->second != sender) {
                it->second->sendReply(message);
            }
        }
    }
}
