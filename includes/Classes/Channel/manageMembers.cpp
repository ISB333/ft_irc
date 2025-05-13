/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : includes/Classes/Channel/manageMembers.cpp
** Author     : aheitz
** Created    : 2025-04-28
** Edited     : 2025-05-13
** Description: All useful functions for managing members in the channel
*/

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

/**
 * @brief Registers a new client on the channel
 * 
 * @param customer The client object to be saved
 */
void Channel::addClient(Client* client) {
    if (not isMember(client->getFd()))
        members_[client->getFd()] = client;
};

/**
 * @brief Removes a client from the channel
 * 
 * @param clientFd The client descriptor in question
 */
void Channel::removeClient(const int clientFd) { operators_.erase(clientFd); members_.erase(clientFd); };

/**
 * @brief Checks whether a client's file descriptor is registered in a channel
 * 
 * @param clientFd The client to check
 * @return true If so
 * @return false Otherwise
 */
bool Channel::isMember(const int clientFd) const { return members_.count(clientFd); };

//TODO: Securing the operation here?
/**
 * @brief Promotes a member to operator rank
 * 
 * @param clientFd The client
 */
void Channel::promoteOperator(const int clientFd) { operators_[clientFd] = members_[clientFd]; };

/**
 * @brief Checks if a client is a channel operator.
 * 
 * @param clientFd The client to check
 * @return true If so
 * @return false Otherwise
 */
bool Channel::isOperator(const int clientFd) const { return operators_.count(clientFd); };

/**
 * @brief Getter to get the complete list of members
 * 
 * @return const map<int, Client*>& The container of members
 */
const map<int, Client*> &Channel::getMembers(void) const { return members_; };

/**
 * @brief Getter to get the complete list of operators
 * 
 * @return const map<int, Client*>& The container of operators
 */
const map<int, Client*> &Channel::getOperators(void) const { return operators_; };

/**
 * @brief Getter to get a specific channel member
 * 
 * @param clientFd The descriptor of a client to search for
 * @return Client* The client object
 */
Client *Channel::getClient(const int clientFd) const { return isMember(clientFd) ? members_.find(clientFd)->second : NULL; };

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

/**
 * @brief Tries client to join a channel, depending on parameters
 * 
 * @param client The client in question
 * @param providedKey A password specified by the client
 */
 void Channel::tryJoin(Client *client, const string &providedKey) {
    if (isMember(client->getFd()))
        throw runtime_error("Client already connected");
    else if (userLimit_ and members_.size() at_least userLimit_) 
        throw runtime_error("Channel already full");
    else if (inviteOnly_ and not isInvited(client->getFd()))
        throw runtime_error("Channel requires an invitation");
    else if (not key_.empty() and providedKey not_eq key_)
        throw runtime_error("A valid password is required");
    else {
        addClient(client);
        if (inviteOnly_)
            removeInvitation(client->getFd());
    }   
};