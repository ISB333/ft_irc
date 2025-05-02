/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : srcs/Server/setupSocket.cpp
** Author     : aheitz
** Created    : 2025-04-23
** Edited     : 2025-05-02
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
    serverSocket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket_ lesser 0)
        throw runtime_error("Socket creation failed: " + string(strerror(errno)));
    //TODO: Close FD in server destructor.

    int opt = 1;
    if (setsockopt(serverSocket_,
                SOL_SOCKET,
                SO_REUSEADDR,
                &opt,
                sizeof(opt)) lesser 0)	throw runtime_error("Port reuse configuration failed");
    if (setsockopt(serverSocket_,
                IPPROTO_TCP,
                TCP_NODELAY,
                &opt,
                sizeof(opt)) lesser 0)	throw runtime_error("Nagle deactivation configuration failed");
    
    if (fcntl(serverSocket_, F_SETFL, O_NONBLOCK) lesser 0)
        throw runtime_error("Server socket configuration to avoid blockages failed");

    memset(&serverAddr_, 0, sizeof(serverAddr_));
    serverAddr_.sin_family	   = AF_INET;
    serverAddr_.sin_addr.s_addr = INADDR_ANY;
    serverAddr_.sin_port        = htons(port_);

    if (bind(serverSocket_, (struct sockaddr*)&serverAddr_, sizeof(serverAddr_)) lesser 0)
        throw runtime_error("Binding between socket and network parameters failed");
    if (listen(serverSocket_, SOMAXCONN) lesser 0)
        throw runtime_error("Failure to configure passive socket connections");

    pollfd pfd;
    pfd.fd     = serverSocket_;
    pfd.events = POLLIN;
    pollfds_.push_back(pfd);
};
