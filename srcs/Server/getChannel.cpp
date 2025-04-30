/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : srcs/Server/getChannel.cpp
** Author     : aheitz
** Created    : 2025-04-24
** Edited     : 2025-04-24
** Description: Definition of getChannel's server function
*/

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

using namespace std;

/**
 * @brief Searches the pointer to a channel in the server
 * 
 * @param name The name of the channel to search for
 * @return Channel* The pointer to this channel
 */
Channel *Server::getChannel(const string &name) const {
    const map<string, Channel*>::const_iterator occurrence = channels.find(name);

    if (occurrence eq channels.end())  throw out_of_range("Channel not found: " + name);
    return occurrence->second;
};
