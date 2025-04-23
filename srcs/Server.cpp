/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adesille <adesille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 11:06:23 by adesille          #+#    #+#             */
/*   Updated: 2025/04/23 14:21:48 by adesille         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "Server.hpp"

Server::Server(int port, const std::string& password) : port(port), password(password) { setupSocket(); }
Server::~Server() { };

void	Server::setupSocket() {
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket < 0) {
		perror("socket");
		exit(1);
	}
	int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    fcntl(serverSocket, F_SETFL, O_NONBLOCK);

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("bind");
        exit(1);
    }

    listen(serverSocket, 5);

    struct pollfd pfd;
    pfd.fd = serverSocket;
    pfd.events = POLLIN;
    pollfds.push_back(pfd);
}

void Server::handleNewConnection() {
    struct sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
    int clientFd = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);

    if (clientFd < 0) {
        perror("accept");
        return;
    }

    fcntl(clientFd, F_SETFL, O_NONBLOCK);

    Client* newClient = new Client(clientFd);
    clients[clientFd] = newClient;

    struct pollfd pfd;
    pfd.fd = clientFd;
    pfd.events = POLLIN;
    pollfds.push_back(pfd);

    std::cout << "New client connected: " << clientFd << std::endl;
}

void Server::run() {
    while (true) {
        int ret = poll(&pollfds[0], pollfds.size(), -1);
        if (ret < 0) {
            perror("poll");
            continue;
        }

        if (pollfds[0].revents & POLLIN) {
            handleNewConnection();
        }

        for (size_t i = 1; i < pollfds.size(); ++i) {
            if (pollfds[i].revents & POLLIN) {
                handleClientData(i);
            }
        }
    }
}

void Server::handleClientData(int index) {
    int fd = pollfds[index].fd;
    Client* client = clients[fd];
    char buffer[512];
    ssize_t bytesRead = recv(fd, buffer, sizeof(buffer), 0);

    if (bytesRead <= 0) {
        removeClient(fd);
        return;
    }

    client->buffer.append(buffer, bytesRead);

    size_t pos;
    while ((pos = client->buffer.find("\r\n")) != std::string::npos) {
        std::string message = client->buffer.substr(0, pos);
        client->buffer.erase(0, pos + 2);
        processCommand(client, message);
    }
}

void Server::processCommand(Client* client, const std::string& message) {
    // Command parsing and processing logic here
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
