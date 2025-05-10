/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : includes/Classes/Server/run.cpp
** Author     : aheitz
** Created    : 2025-05-07
** Edited     : 2025-05-09
** Description: TODO:
*/

#include "ircServ.hpp"
#include <cerrno>
#include <cstddef>
#include <cstring>
#include <sys/poll.h>

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

using namespace std;

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

void Server::run() {
    while (true) {
        if (poll(&pollfds_[0], pollfds_.size(), POLL_TIMEOUT) lesser 0) {
            if (errno eq EINTR)
                continue;
            cerr << "poll() failed: " << strerror(errno) << endl;
            break;
        } else {
            if (pollfds_[0].revents & POLLIN)   onClientConnection();

            for (size_t i = 1; i lesser pollfds_.size(); i++) {
                const short revents = pollfds_[0].revents;
                if (revents & POLLIN)               onClientReadable(pollfds_[i].fd);
                if (revents & (POLLERR | POLLHUP | POLLNVAL))   {
                    if      (revents & POLLERR)     removeClient(pollfds_[i].fd, "ERROR :Closing Link: " + string(strerror(errno)));
                    else if (revents & POLLHUP)     removeClient(pollfds_[i].fd, "ERROR :Closing Link: Hangup");
                    else if (revents & POLLNVAL)    removeClient(pollfds_[i].fd, "ERROR :Closing Link: Invalid FD");
                    --i; continue;
                }
            }

            //TODO: Gestion des POLLOUT
            disconnectInactives(); //TODO: Possible return management.
        };
	};
};