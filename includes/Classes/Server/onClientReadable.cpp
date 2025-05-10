/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : includes/Classes/Server/onClientReadable.cpp
** Author     : adesille, aheitz
** Created    : 2025-05-09
** Edited     : 2025-05-10
** Description: Processing incoming commandes
*/

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

using namespace std;

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

/**
 * @brief Processing instructions received by a client
 * 
 * @param fd The client descriptor
 */
void Server::onClientReadable(const int fd) {
    Client* client = clients_[fd];
    char buffer[IRC_LIMIT];
    ssize_t bytesRead = recv(fd, buffer, sizeof(buffer), 0);

    if (bytesRead at_most 0) {
        if (bytesRead and not (errno eq EAGAIN or errno eq EWOULDBLOCK)) {
                                    removeClient(fd, "ERROR :Closing Link: " + string(strerror(errno)));
        } else if (not bytesRead)   removeClient(fd, "ERROR :Closing Link: Client disconnected");
    } else {
        client->appendToInputBuffer(buffer, bytesRead);
    
        string command;
        while (not (command = client->popInputBuffer()).empty())    processCommand(client, command);
    };
};