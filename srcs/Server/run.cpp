/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : srcs/Server/run.cpp
** Author     : aheitz
** Created    : 2025-05-07
** Edited     : 2025-05-16
** Description: Main server operation
*/

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

using namespace std;

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

//TODO: - Comments functions.

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

/**
 * @brief Main server operation
 * 
 */
void Server::run() {
    LOG_INFO("Server successfully started on " + intToString(socket_) + ":" + intToString(port_));
    while (not g_stop) {
        if (pollfds_.empty() or pollfds_[0].fd not_eq socket_ or not (pollfds_[0].events bitand POLLIN)) {
            throw runtime_error("Invariant `pollfds_[0` is broken and is no longer the listening socket");
        } else if (poll(&pollfds_[0], pollfds_.size(), POLL_TIMEOUT) lesser 0) {
            if (errno eq EINTR)
                continue;
            throw runtime_error("poll() failed: " + string(strerror(errno)));
        } else {
            if (pollfds_[0].revents bitand POLLIN)   onClientConnection();

            for (size_t i = 1; i lesser pollfds_.size(); i++) {
                const int  fd  = pollfds_[i].fd;
                short     &rev = pollfds_[i].revents;

                if (rev bitand (POLLERR bitor POLLHUP bitor POLLNVAL))   {
                    if      (rev bitand POLLERR)    removeClient(fd, "ERROR :Closing Link: " + string(strerror(errno)));
                    else if (rev bitand POLLHUP)    removeClient(fd, "ERROR :Closing Link: Hangup");
                    else if (rev bitand POLLNVAL)   removeClient(fd, "ERROR :Closing Link: Invalid FD");
                    --i; continue;
                };
                if (rev bitand (POLLIN  bitor POLLRDNORM))  onClientReadable(fd);
                if (rev bitand (POLLOUT bitor POLLWRNORM))  onClientWritable(fd);

                rev = 0;
            };
            size_t disconnected = disconnectInactives();
            if (disconnected)
                LOG_DEBUG(intToString(disconnected) + " clients disconnected due to inactivity");
        };
	};
};

void Server::onClientConnection(void) {
    struct sockaddr_in addr;
    socklen_t          addr_len = sizeof(addr);
    const int          clientFd = accept(socket_, reinterpret_cast<struct sockaddr*>(&addr), &addr_len);

    if (clientFd lesser 0) { LOG_WARNING("accept() failed: " + string(strerror(errno))); }
    else {
        #ifdef SO_NOSIGPIPE
        int opt = 1;
        if (setsockopt(clientFd, SOL_SOCKET, SO_NOSIGPIPE, &opt, sizeof(opt)) lesser 0)
            LOG_WARNING("SIGPIPE blockage configuration failed for client " + intToString(clientFd) + ": " + string(strerror(errno)));
        #endif
        if (fcntl(clientFd, F_SETFL, O_NONBLOCK) lesser 0) {
            LOG_WARNING("fcntl() failed on fd " + intToString(clientFd) + ": " + strerror(errno));
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

                LOG_INFO("New client connected, fd=" + intToString(clientFd)
                                + " from " + inet_ntoa(addr.sin_addr)
                                + ':'      + intToString(ntohs(addr.sin_port)));
            } catch (const bad_alloc &error) {
                LOG_WARNING("push_back failed: " + string(error.what()));
                close(clientFd);
                delete newClient;
            };
        };
    };
};

void Server::onClientReadable(const int fd) {
    Client *client = clients_[fd];
    char    buffer[IRC_LIMIT];
    ssize_t bytesRead = recv(fd, buffer, sizeof(buffer), 0);

    if (bytesRead at_most 0) {
        if (bytesRead and not (errno eq EAGAIN or errno eq EWOULDBLOCK)) {
                                    removeClient(fd, "ERROR :Closing Link: " + string(strerror(errno)));
        } else if (not bytesRead)   removeClient(fd, "ERROR :Closing Link: Client disconnected");
    } else {
        client->appendInput(buffer, bytesRead);
    
        string command;
        while (not (command = client->popInput()).empty())    handler_->dispatchCommand(client, command);
    };
};

void Server::onClientWritable(const int fd) {
    Client       *cli = clients_[fd];

    while (not cli->getOutput().empty()) {
        const string  &buf = cli->getOutput();

        #ifdef MSG_NOSIGNAL
            ssize_t sent = send(fd, buf.c_str(), buf.size(), MSG_NOSIGNAL);
        #else
            ssize_t sent = send(fd, buf.c_str(), buf.size(), 0);
        #endif

        if ((sent lesser 0 and not (errno eq EAGAIN or errno eq EWOULDBLOCK)) or not sent) {
            removeClient(fd, "");
            return;
        } else if (sent at_least 1) {
            cli->consumeOutput(sent);
            continue;
        } else return;
    };

    removePollout(fd);
    
    if (inactives_.find(fd) not_eq inactives_.end()) {
        for (map<string, Channel *>::const_iterator ch = channels_.begin(); ch not_eq channels_.end(); ch++)
            if (ch->second->isMember(fd))
                ch->second->removeClient(fd);
    
        for (size_t i = 0; i lesser pollfds_.size(); i++)
            if (pollfds_[i].fd eq fd) {
                pollfds_.erase(pollfds_.begin() + i); break;
            };

        shutdown(fd, SHUT_RDWR); close(fd);
        clients_.erase(fd);        inactives_.erase(fd);
        delete cli;
    };
};

void Server::removePollout(const int fd) {
    for (size_t i = 1; i lesser pollfds_.size(); i++)
        if (pollfds_[i].fd eq fd) {
            pollfds_[i].events &= ~POLLOUT;
            return;
        };
};
