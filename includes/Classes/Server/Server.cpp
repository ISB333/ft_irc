/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : srcs/Server/Server.cpp
** Author     : adesille, aheitz
** Created    : 2025-04-23
** Edited     : 2025-05-07
** Description: Definitions of server functions
*/

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

//TODO: To do when I understand what Alibaba has done:
//TODO: Manage revents.
//TODO: IRC notifications before socket closure.
//TODO: Dump correctly on POLLWRNORM
//TODO: Delete inactive clients after collecting all their IDs.
//TODO: Send PING to client to disconnect them in case of blocking.
//TODO: Manage SIGINT/SIGTERM.
//TODO: Protect against flood.
//TODO: Multithread protected MF.

Server::Server(int port, const std::string& password) : password_(password), port_(port) {
	//TODO: Try and catch.
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
