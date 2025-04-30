/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : srcs/Server/Server.cpp
** Author     : adesille, aheitz
** Created    : 2025-04-23
** Edited     : 2025-04-24
** Description: Definitions of server functions
*/

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

Server::Server(int port, const std::string& password) : password(password), port(port) {
	setupSocket();
	handler = new Handler(*this);
}

Server::~Server() {
    for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
        delete it->second;
    }
    for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
        delete it->second;
    }
	delete handler;
    close(serverSocket);
};

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
	client->appendToInputBuffer(buffer, bytesRead);
	// client->_buffer.append(buffer, bytesRead);

	// IRC messages are terminated by \r\n
	// Process complete messages while leaving partial messages in buffer

	std::string s = client->popInputBuffer();		
	if (!s.empty())
		processCommand(client, s);
	// size_t pos;
	// while ((pos = client->getInputBuffer().find("\r\n")) != std::string::npos) {
	// 	std::string message = client->getInputBuffer().substr(0, pos);
	// 	client->buffer.erase(0, pos + 2);  // Remove processed message including \r\n
	// 	processCommand(client, message);
	// }
}

void Server::processCommand(Client* client, const std::string& message) {
	/*
	{
    	// Command parsing and processing logic here
	}
	*/
    // std::cout << "Received: " << message << std::endl;
	// std::cout << "Received: ";
	// Command cmd = parseLine(message);
	// if (!cmd.name.empty())
	// 	std::cout << "name: " << cmd.name << std::endl;
	// else
	// 	std::cout << "name empty" << std::endl;
	// if (!cmd.argv.empty())
	// 	std::cout << "cmd: " << cmd.argv[0] << std::endl;
	// else
	// 	std::cout << "cmd empty" << std::endl;
    // client->sendReply(":" + client->nickname + "!" + client->username + "@localhost " + message + "\r\n");
	handler->dispatchCommand(client, message);
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
		std::map<int, Client*> members = channel->getMembers();
        for (std::map<int, Client*>::iterator it = members.begin(); it != members.end(); ++it) {
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

void	Server::authentification(std::string passwd) {
	if (passwd == password)
		// allow client
		std::cout << "Allowing Client to Enter in Server" << std::endl;
}
