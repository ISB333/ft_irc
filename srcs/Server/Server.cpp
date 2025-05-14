/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : srcs/Server/Server.cpp
** Author     : adesille, aheitz
** Created    : 2025-04-23
** Edited     : 2025-05-14
** Description: Definitions of server functions
*/

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

//TODO: Dump correctly on POLLWRNORM
//TODO: Delete inactive clients after collecting all their IDs.
//TODO: Send PING to client to disconnect them in case of blocking.
//TODO: Manage SIGINT/SIGTERM.
//TODO: Protect against flood.
//TODO: Multithread protected MF.
//TODO: If accept() fails in a loop, rekt this *.
//TODO: Switch logs on a DEBUG/INFO/etc. structure?

/**
 * @brief Construct a new Server:: Server object
 * 
 * @param port The port used by the server
 * @param password The password used by the server
 */
Server::Server(const int port, const std::string& password) : port_(port), password_(password),
                                                              socket_(-1), address_(),
                                                              handler_(new Handler(*this)),
                                                              pollfds_(),  channels_(),
                                                              clients_(),  banned_() {
    signal(SIGPIPE, SIG_IGN);
    setupSocket();
};

/**
 * @brief Destroy the Server:: Server object
 * 
 */
Server::~Server(void) {
    //TODO: If you have too much time, don't hesitate to use the catches.
    for (std::map<int, Client*>::iterator cli = clients_.begin(); cli not_eq clients_.end(); cli++) {
        try         { shutdown(cli->second->getFd(), SHUT_RDWR); delete cli->second; }
        catch (...) {                                                                                    };
    }
    for (std::map<std::string, Channel*>::iterator ch = channels_.begin(); ch not_eq channels_.end(); ch++)
        try         { delete ch->second; }
        catch (...) {                    };
    if (socket_ not_eq -1)  close(socket_);
    cerr << "Server socket closed" << endl;
};

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
        throw runtime_error("Port reuse configuration failed: " + string(strerror(errno)));
    if (setsockopt(socket_, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt)) lesser 0)
        throw runtime_error("Nagle deactivation configuration failed: " + string(strerror(errno)));
    #ifdef SO_NOSIGPIPE
        if (setsockopt(socket_, SOL_SOCKET, SO_NOSIGPIPE, &opt, sizeof(opt)) lesser 0)
            throw runtime_error("SIGPIPE blockage configuration failed: " + string(strerror(errno)));
    #endif

    if (fcntl(socket_, F_SETFL, O_NONBLOCK) lesser 0)
        throw runtime_error("Server socket configuration to avoid blockages failed: " + string(strerror(errno)));

    memset(&address_, 0, sizeof(address_));
    address_.sin_family	     = AF_INET;
    address_.sin_addr.s_addr = INADDR_ANY;
    address_.sin_port        = htons(port_);

    if (bind(socket_, (struct sockaddr*)&address_, sizeof(address_)) lesser 0)
        throw runtime_error("Binding between socket and network parameters failed: " + string(strerror(errno)));
    if (listen(socket_, SOMAXCONN) lesser 0)
        throw runtime_error("Failure to configure passive socket connections: " + string(strerror(errno)));

    pollfd pfd;
    pfd.fd      = socket_;
    pfd.events  = POLLIN;
    pfd.revents = 0;
    pollfds_.push_back(pfd);
};

/**
 * @brief Searches the pointer to a channel in the server
 * 
 * @param name The name of the channel to search for
 * @return Channel* The pointer to this channel
 */
Channel *Server::getChannel(const string &name) const {
    const map<string, Channel*>::const_iterator occurrence = channels_.find(name);

    if (occurrence eq channels_.end())  throw out_of_range("Channel not found: " + name);
    return occurrence->second;
};

void Server::reply(Client *cli, const std::string &msg) {
    if (not msg.empty())
        cli->appendOutput(msg);
    for (size_t i = 1; i lesser pollfds_.size(); i++)
        if (pollfds_[i].fd        eq cli->getFd()) {
            pollfds_[i].events or_eq POLLOUT;
            break;
        };
};