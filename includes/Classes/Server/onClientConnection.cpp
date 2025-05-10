/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : includes/Classes/Server/onClientConnection.cpp
** Author     : adesille, aheitz
** Created    : 2025-05-09
** Edited     : 2025-05-10
** Description: New client connection management
*/

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

using namespace std;

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

/**
 * @brief Registers and initializes a new client on the server
 * 
 */
void Server::onClientConnection(void) {
    struct sockaddr_in addr;
    socklen_t          addr_len = sizeof(addr);
    const int          clientFd = accept(socket_, reinterpret_cast<struct sockaddr*>(&addr), &addr_len);

    if (clientFd lesser 0) { cerr << "accept() failed: " << strerror(errno) << endl; }
    else {
        if (fcntl(clientFd, F_SETFL, O_NONBLOCK) lesser 0) {
            cerr << "fcntl() failed on fd " << clientFd << ": " << strerror(errno) << endl;
            close(clientFd);
        } else {
            Client *newClient  = new Client(clientFd);
            clients_[clientFd] = newClient;
            newClient->updateActivity();
        
            struct pollfd pfd;
            pfd.fd      = clientFd;
            pfd.events  = POLLIN;
            pfd.revents = 0;
            try {
                pollfds_.push_back(pfd);

                cout << "New client connected, fd=" << clientFd
                     << " from " << inet_ntoa(addr.sin_addr)
                     << ":" << ntohs(addr.sin_port) << endl;
            } catch (const bad_alloc &error) {
                cerr << "push_back failed: " << error.what() << endl;
                close(clientFd);
                delete newClient;
            };
        };
    };
};