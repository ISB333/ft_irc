/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : srcs/Channel.cpp
** Author     : adesille, aheitz
** Created    : 2025-04-23
** Edited     : 2025-04-25
** Description: Channel class member functions
*/

#include "ircServ.hpp"

Channel::Channel(const std::string& name) : name(name), inviteOnly(false), topicRestricted(false), userLimit(-1) {}

void Channel::addClient(Client* client) {
    members[client->getFileDescriptor()] = client;
}

void Channel::removeClient(Client* client) {
    members.erase(client->getFileDescriptor());
    operators.erase(client->getFileDescriptor());
}

void Channel::setMode(char mode, bool set, const std::string& arg) {
    switch (mode) {
        case 'i': inviteOnly = set; break;
        case 't': topicRestricted = set; break;
        case 'k': if (set) key = arg; else key = ""; break;
        case 'l': if (set) userLimit = atoi(arg.c_str()); else userLimit = -1; break;
        case 'o': {
            if (set) operators[atoi(arg.c_str())] = members[atoi(arg.c_str())];
            else operators.erase(atoi(arg.c_str()));
            break;
        }
    }

}

/**
 * @brief Checks whether a client's file descriptor is registered in a channel
 * 
 * @param clientFd The client to check
 * @return true If the client is indeed registered in the channel
 * @return false If passed fd is not present in the channel
 */
bool Channel::isMember(const int clientFd) const {
    return members.count(clientFd);
};