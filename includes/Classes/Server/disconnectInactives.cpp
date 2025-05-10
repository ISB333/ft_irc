/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : includes/Classes/Server/disconnectInactives.cpp
** Author     : aheitz
** Created    : 2025-05-07
** Edited     : 2025-05-07
** Description: Analyze and deletion of inactive clients
*/

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

using namespace std;

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

/**
 * @brief Analyzes each client and remove inactive ones
 * 
 * @return size_t The number of disconnected clients
 */
size_t Server::disconnectInactives(void) {
    const time_t now = time(NULL);
    vector<int>  toDisconnect;   toDisconnect.reserve(clients_.size());

    for (map<int, Client *>::const_iterator it = clients_.begin(); it not_eq clients_.end(); it++)
        if (now - it->second->getLastActivity() greater INACTIVITY_SECONDS)
        toDisconnect.push_back(it->first);

    for (vector<int>::const_iterator it = toDisconnect.begin(); it not_eq toDisconnect.end(); it++)
        removeClient(*it, "ERROR :Closing Link: Idle timeout");

    return toDisconnect.size();
};