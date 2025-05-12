/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : srcs/Server/run.cpp
** Author     : aheitz
** Created    : 2025-05-07
** Edited     : 2025-05-12
** Description: Main server operation
*/

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

using namespace std;

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

//TODO: Check that pollfds[0] is the listening socket.
//TODO: Manage SIGPIPE.
//TODO: Reset revent to 0.

/**
 * @brief Main server operation
 * 
 */
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
                const short revents = pollfds_[i].revents;
                const int   fd      = pollfds_[i].fd;

                if (revents & (POLLERR | POLLHUP | POLLNVAL))   {
                    if      (revents & POLLERR)     removeClient(fd, "ERROR :Closing Link: " + string(strerror(errno)));
                    else if (revents & POLLHUP)     removeClient(fd, "ERROR :Closing Link: Hangup");
                    else if (revents & POLLNVAL)    removeClient(fd, "ERROR :Closing Link: Invalid FD");
                    --i; continue;
                };
                if (revents & POLLIN)               onClientReadable(fd);
                if (revents & POLLOUT)              onClientWritable(fd);
            };
            disconnectInactives(); //TODO: Possible return management.
        };
	};
};

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

/**
 * @brief Processing instructions received by a client
 * 
 * @param fd The client descriptor
 */
void Server::onClientReadable(const int fd) {
    Client *client = clients_[fd];
    char    buffer[IRC_LIMIT];
    ssize_t bytesRead = recv(fd, buffer, sizeof(buffer), 0);

    if (bytesRead at_most 0) {
        if (bytesRead and not (errno eq EAGAIN or errno eq EWOULDBLOCK)) {
                                    removeClient(fd, "ERROR :Closing Link: " + string(strerror(errno)));
        } else if (not bytesRead)   removeClient(fd, "ERROR :Closing Link: Client disconnected");
    } else {
        client->appendToInputBuffer(buffer, bytesRead);
    
        string command;
        while (not (command = client->popInputBuffer()).empty())    handler_->dispatchCommand(client, command);
    };
};

/**
 * @brief Processing instructions sent by a client
 * 
 * @param fd The client descriptor
 */
void Server::onClientWritable(const int fd) {
    Client       *cli = clients_[fd];
    const string &buf = cli->getOutputBuffer();

    if (buf.empty()) { removePollout(fd); }
    else if (not buf.empty()) {
        const ssize_t sent = send(fd, buf.c_str(), buf.size(), 0);
        if (sent lesser 0 and not (errno eq EAGAIN or errno eq EWOULDBLOCK)) {
            removeClient(fd, "ERROR :Closing Link: " + string(strerror(errno)));
        } else if (sent eq 0) {
            removeClient(fd, "ERROR :Closing Link: connection closed by peer");
        } else if (sent at_least 1) {
            cli->consumeOutput(sent);
            if (cli->getOutputBuffer().empty()) removePollout(fd);
        };
    };
};

/**
 * @brief Removes POLLOUT flag from socket events
 * 
 * @param fd The descriptor to remove POLLOUT from
 */
void Server::removePollout(const int fd) {
    for (size_t i = 1; i lesser pollfds_.size(); i++)
        if (pollfds_[i].fd eq fd) {
            pollfds_[i].events &= ~POLLOUT;
            return;
        };
};