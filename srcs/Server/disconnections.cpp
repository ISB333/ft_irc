/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : srcs/Server/disconnections.cpp
** Author     : aheitz
** Created    : 2025-05-07
** Edited     : 2025-05-13
** Description: Analyze and deletion of inactive clients
*/

#include "ircServ.hpp"
#include <sys/poll.h>

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

    for (map<int, Client *>::const_iterator it = clients_.begin(); it not_eq clients_.end(); it++) {
        const time_t delta = now - it->second->getActivity();
        if (delta lesser 0 or delta greater INACTIVITY_SECONDS) {
            toDisconnect.push_back(it->first);
            cerr << "Client " << it->first << " marked as inactive and will be removed" << endl;
        };
    };

    for (vector<int>::const_iterator it = toDisconnect.begin(); it not_eq toDisconnect.end(); it++)
        removeClient(*it, "ERROR :Closing Link: Idle timeout");

    return toDisconnect.size();
};

void Server::removeClient(const int fd, const string &reason) {
    map<int, Client*>::iterator cli = clients_.find(fd);
    if (cli not_eq clients_.end()) {
        if (not reason.empty())
            reply(cli->second, reason);
        inactives_.insert(cli->second->getFd());
    };
};