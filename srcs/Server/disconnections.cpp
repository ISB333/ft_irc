/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : srcs/Server/disconnections.cpp
** Author     : aheitz
** Created    : 2025-05-07
** Edited     : 2025-05-12
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

    for (map<int, Client *>::const_iterator it = clients_.begin(); it not_eq clients_.end(); it++) {
        const time_t delta = now - it->second->getLastActivity();
        if (delta lesser 0 or delta greater INACTIVITY_SECONDS) {
            toDisconnect.push_back(it->first);
            cerr << "Client " << it->first << " marked as inactive and will be removed" << endl;
        };
    };

    for (vector<int>::const_iterator it = toDisconnect.begin(); it not_eq toDisconnect.end(); it++)
        removeClient(*it, "ERROR :Closing Link: Idle timeout");

    return toDisconnect.size();
};

/**
 * @brief Removes a client from the server and its channels and notifies it.
 * 
 * @param fd The client to remove
 * @param reason The message to justify the disconnection
 */
void Server::removeClient(const int fd, const string &reason) {
    try {
        map<int, Client*>::iterator it = clients_.find(fd);
        if (it eq clients_.end())
            return;
        Client *cli = it->second;
    
        if (not reason.empty())
            cli->sendReply(reason);
    
        for (map<string, Channel *>::const_iterator ch = channels_.begin(); ch not_eq channels_.end(); ch++)
            if (ch->second->isMember(fd))
                ch->second->removeClient(fd);
    
        for (size_t i = 0; i lesser pollfds_.size(); i++)
            if (pollfds_[i].fd eq fd) {
                pollfds_.erase(pollfds_.begin() + i); break;
            }
    
        shutdown(fd, SHUT_RDWR); close(fd);
        clients_.erase(it); delete cli;
    } catch (const exception &error) {
        cerr << "removeClient() failed: " << error.what()   << endl;
    } catch (...) {
        cerr << "removeClient() failed (unknown exception)" << endl;
    };
};