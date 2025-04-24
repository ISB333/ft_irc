/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : srcs/Server/setupSocket.cpp
** Author     : aheitz
** Created    : 2025-04-23
** Edited     : 2025-04-24
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
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket lesser 0)
        throw runtime_error("Socket creation failed: " + string(strerror(errno)));
    //TODO: Close FD in server destructor.

    int opt = 1;
    if (setsockopt(serverSocket,
                SOL_SOCKET,
                SO_REUSEADDR,
                &opt,
                sizeof(opt)) lesser 0)	throw runtime_error("Port reuse configuration failed");
    if (setsockopt(serverSocket,
                IPPROTO_TCP,
                TCP_NODELAY,
                &opt,
                sizeof(opt)) lesser 0)	throw runtime_error("Nagle deactivation configuration failed");
    
    if (fcntl(serverSocket, F_SETFL, O_NONBLOCK) lesser 0)
        throw runtime_error("Server socket configuration to avoid blockages failed");

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family	   = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port        = htons(port);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) lesser 0)
        throw runtime_error("Binding between socket and network parameters failed");
    if (listen(serverSocket, SOMAXCONN) lesser 0)
        throw runtime_error("Failure to configure passive socket connections");

    pollfd pfd;
    pfd.fd     = serverSocket;
    pfd.events = POLLIN;
    pollfds.push_back(pfd);
};