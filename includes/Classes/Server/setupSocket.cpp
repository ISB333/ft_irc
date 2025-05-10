/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : includes/Classes/Server/setupSocket.cpp
** Author     : aheitz
** Created    : 2025-04-23
** Edited     : 2025-05-10
** Description: Function to initiate the server socket
*/

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

using namespace std;

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

/**
 * @brief Server member function for socket configuration
 * 
 * Creates a socket for IPv4, defines port reuse and disables Nagle.
 * Prevents blockages, defines the network and links it to the socket.
 * Then passively listens for connections and finally, initiates monitoring of client connections.
 *
*/
void Server::setupSocket(void) {
    socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_ lesser 0)
        throw runtime_error("Socket creation failed: " + string(strerror(errno)));

    int opt = 1;
    if (setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) lesser 0)
        throw runtime_error("Port reuse configuration failed");
    if (setsockopt(socket_, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt)) lesser 0)
        throw runtime_error("Nagle deactivation configuration failed");

    if (fcntl(socket_, F_SETFL, O_NONBLOCK) lesser 0)
        throw runtime_error("Server socket configuration to avoid blockages failed");

    memset(&socket_, 0, sizeof(address_));
    address_.sin_family	     = AF_INET;
    address_.sin_addr.s_addr = INADDR_ANY;
    address_.sin_port        = htons(port_);

    if (bind(socket_, (struct sockaddr*)&address_, sizeof(address_)) lesser 0)
        throw runtime_error("Binding between socket and network parameters failed");
    if (listen(socket_, SOMAXCONN) lesser 0)
        throw runtime_error("Failure to configure passive socket connections");

    pollfd pfd;
    pfd.fd      = socket_;
    pfd.events  = POLLIN;
    pfd.revents = 0;
    pollfds_.push_back(pfd);
};