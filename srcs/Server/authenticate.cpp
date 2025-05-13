/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : srcs/Server/authentication.cpp
** Author     : adesille, aheitz
** Created    : 2025-05-12
** Edited     : 2025-05-12
** Description: Advanced client authentication management
*/

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

using namespace std;

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

//TODO: --- Non-exhaustive list of potential improvements: ---
//TODO:  - removeClient and force_disconnect repeat shutdown() and close().
//TODO:  - Magic numbers 2 and 3 could be literal macros.
//TODO:  - Secure this little * of setcockopt()!
//FIXME: - Output should be cerr, not out (INFO/DEBUG/ERROR structure).
//FIXME: - The order of the conditions lets the client connect if the password is correct. Even if it's ban IP.
//TODO:  - Define ban reasons.
//FIXME: - removeClient() sends a message. With force_disconnect() just before, crash is inevitable, review instruction order.

void    force_disconnect(int fd)
{
    shutdown(fd, SHUT_WR);
    struct linger  so_linger;

    so_linger.l_onoff  = 1;
    so_linger.l_linger = 0;

    setsockopt(fd, SOL_SOCKET, SO_LINGER,
               &so_linger, sizeof(so_linger));

    close(fd);
}

void Server::authenticate(Client *client, const std::string &password) {
    if (password == password_) {
        std::cout << "Password Correct, you are allowed to enter in the Server" << std::endl;
        client->authenticate(true);
    }
    else if (isBanned(client->getIp())) {
        client->sendReply(Replies::ERR_ALREADYBANNED());
        force_disconnect(client->getFd());
        removeClient(client->getFd(), "TO DEFINE");
    }
    else {
        client->incrementAttempt();
        int passwdAttempt = client->getAttempt();
        if (passwdAttempt == 3) {
             client->sendReply(Replies::ERR_PASSWDMISMATCH(password, passwdAttempt));
            client->sendReply(Replies::ERR_YOUREBANNEDCREEP());
            ban(client->getIp());
            force_disconnect(client->getFd());
            removeClient(client->getFd(), "TO DEFINE");
        }
        else if (passwdAttempt == 2) {
             client->sendReply(Replies::ERR_PASSWDMISMATCH(password, passwdAttempt));
             client->sendReply(Replies::ERR_YOUWILLBEBANNED());
        }
        else
             client->sendReply(Replies::ERR_PASSWDMISMATCH(password, passwdAttempt));
    }
}

/**
 * @brief Checks if the associated IP is banned from the server
 */
bool Server::isBanned(const string &ip) const { return banned_.find(ip) not_eq banned_.end(); };

/**
 * @brief Registers an IP to block future client connections
 */
void Server::ban(const string &ip) { banned_.insert(ip); };