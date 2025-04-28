/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : includes/Classes/Channel/manageMembers.cpp
** Author     : aheitz
** Created    : 2025-04-28
** Edited     : 2025-04-28
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
    if (not isMember(client->getFileDescriptor()))
        _members[client->getFileDescriptor()] = client;
};

/**
 * @brief Removes a client from the channel
 * 
 * @param clientFd The client descriptor in question
 */
void Channel::removeClient(const int clientFd) { _operators.erase(clientFd); _members.erase(clientFd); };

/**
 * @brief Checks whether a client's file descriptor is registered in a channel
 * 
 * @param clientFd The client to check
 * @return true If so
 * @return false Otherwise
 */
bool Channel::isMember(const int clientFd) const { return _members.count(clientFd); };

/**
 * @brief Checks if a client is a channel operator.
 * 
 * @param clientFd The client to check
 * @return true If so
 * @return false Otherwise
 */
bool Channel::isOperator(const int clientFd) const { return _operators.count(clientFd); };

/**
 * @brief Getter to get the complete list of members
 * 
 * @return const map<int, Client*>& The container of members
 */
const map<int, Client*> &Channel::getMembers(void) const { return _members; };

/**
 * @brief Getter to get the complete list of operators
 * 
 * @return const map<int, Client*>& The container of operators
 */
const map<int, Client*> &Channel::getOperators(void) const { return _operators; };

/**
 * @brief Getter to get a specific channel member
 * 
 * @param clientFd The descriptor of a client to search for
 * @return Client* The client object
 */
Client *Channel::getClient(const int clientFd) const { return isMember(clientFd) ? _members.find(clientFd)->second : NULL; };

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

/**
 * @brief Tries client to join a channel, depending on parameters
 * 
 * @param client The client in question
 * @param providedKey A password specified by the client
 */
 void Channel::tryJoin(Client *client, const string &providedKey) {
    if (isMember(client->getFileDescriptor()))
        throw runtime_error("Client already connected");
    else if (_userLimit and _members.size() at_least _userLimit) 
        throw runtime_error("Channel already full");
    else if (_inviteOnly and not isInvited(client->getFileDescriptor()))
        throw runtime_error("Channel requires an invitation");
    else if (not _key.empty() and providedKey not_eq _key)
        throw runtime_error("A valid password is required");
    else {
        addClient(client);
        if (_inviteOnly)
            removeInvitation(client->getFileDescriptor());
    }   
};