/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : srcs/Server/Server.cpp
** Author     : adesille, aheitz
** Created    : 2025-04-23
** Edited     : 2025-05-02
** Description: Definitions of server functions
*/

#include "ircServ.hpp"
#include <sys/poll.h>

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

Server::Server(int port, const std::string& password) : password_(password), port_(port) {
	setupSocket();
	handler_ = new Handler(*this);
}

Server::~Server() {
    for (std::map<int, Client*>::iterator it = clients_.begin(); it != clients_.end(); ++it) {
        delete it->second;
    }
    for (std::map<std::string, Channel*>::iterator it = channels_.begin(); it != channels_.end(); ++it) {
        delete it->second;
    }
	delete handler_;
    close(serverSocket_);
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
	int clientFd = accept(serverSocket_, (struct sockaddr*)&clientAddr, &clientLen);

	if (clientFd < 0) {
		perror("accept");
		return;
	}

	// Set client socket to non-blocking mode
	fcntl(clientFd, F_SETFL, O_NONBLOCK);

	// Create a new Client object and add it to the clients map using the fd as key
	Client* newClient = new Client(clientFd);
	clients_[clientFd] = newClient;

	// Add the new client socket to the poll monitoring list
	struct pollfd pfd;
	pfd.fd = clientFd;
	pfd.events = POLLIN;  // Monitor for incoming data from this client
	pollfds_.push_back(pfd);

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
		int ret = poll(&pollfds_[0], pollfds_.size(), -1);
		if (ret < 0) {
			perror("poll");
			continue;
		}

		// Check if the server socket has activity (index 0 in pollfds) - this means a new connection
		if (pollfds_[0].revents & POLLIN) {
			handleNewConnection();
		}

		// Check all client sockets for activity
		// Starting from index 1 because index 0 is the server socket
		for (size_t i = 1; i < pollfds_.size(); ++i) {
			if (pollfds_[i].revents & POLLIN) {
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
	
	int fd = pollfds_[index].fd;
	Client* client = clients_[fd];
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

	// IRC messages are terminated by \r\n
	// Process complete messages while leaving partial messages in buffer
	std::string s = client->popInputBuffer();		
	if (!s.empty())
		processCommand(client, s);
}

void Server::processCommand(Client* client, const std::string& message) {
	/*
	{
    	// Command parsing and processing logic here
	}
	*/
    std::cout << "Received: " << message << std::endl;
	std::cout << "Received: ";
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
	handler_->dispatchCommand(client, message);
}

void Server::removeClient(int fd) {
    std::map<int, Client*>::iterator it = clients_.find(fd);
    if (it != clients_.end()) {
        delete it->second;
        clients_.erase(it);
    }

    for (size_t i = 0; i < pollfds_.size(); ++i) {
        if (pollfds_[i].fd == fd) {
            pollfds_.erase(pollfds_.begin() + i);
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
        for (std::map<int, Client*>::iterator it = clients_.begin(); it != clients_.end(); ++it) {
            if (it->second != sender) {
                it->second->sendReply(message);
            }
        }
    }
}

#include <sys/socket.h>
#include <unistd.h>

// ...

void    force_disconnect(int fd)
{
	shutdown(fd, SHUT_WR);
    struct linger  so_linger;

    so_linger.l_onoff  = 1;
    so_linger.l_linger = 0;

    setsockopt(fd, SOL_SOCKET, SO_LINGER,
               &so_linger, sizeof(so_linger));

    close(fd);
}

void	Server::authentification(Client* client, std::string passwd) {
	if (passwd == password_) {
		std::cout << "Password Correct, you are allowed to enter in the Server" << std::endl;
		client->toggleAuthentication(true);
	}
	else if (isIPBanned(client->getIP())) {
		client->sendReply(Replies::ERR_ALREADYBANNED());
		force_disconnect(client->getFileDescriptor());
		removeClient(client->getFileDescriptor());
	}
	else {
		client->incrementPasswdAttempt();
		int passwdAttempt = client->getPasswdAttempt();
		if (passwdAttempt == 3) {
		 	client->sendReply(Replies::ERR_PASSWDMISMATCH(passwd, passwdAttempt));
			client->sendReply(Replies::ERR_YOUREBANNEDCREEP());
			addIPToBanList(client->getIP());
			force_disconnect(client->getFileDescriptor());
			removeClient(client->getFileDescriptor());
		}
		else if (passwdAttempt == 2) {
		 	client->sendReply(Replies::ERR_PASSWDMISMATCH(passwd, passwdAttempt));
		 	client->sendReply(Replies::ERR_YOUWILLBEBANNED());
		}
		else
		 	client->sendReply(Replies::ERR_PASSWDMISMATCH(passwd, passwdAttempt));
	}
}
