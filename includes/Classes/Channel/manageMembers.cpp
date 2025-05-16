/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : includes/Classes/Channel/manageMembers.cpp
** Author     : aheitz
** Created    : 2025-04-28
** Edited     : 2025-05-16
** Description: All useful functions for managing members in the channel
*/

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

#define ERR_MEMBER "already member"
#define ERR_LIMIT  "channel is full"
#define ERR_INVITE "not yet invited"
#define ERR_KEY    "unable to provide keyword"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

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
bool Channel::isMember(const int fd) const { return members_.count(fd); };

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
 * Tries to join a server, throws a runtime exception if the client is not accepted
 */
 void Channel::join(Client *cli, const string &providedKey) {
    int    const fd       = cli->getFd();
    string const nickname = cli->getNickname();
    string            err;

    if (isMember(fd))                                               err = ERR_MEMBER;
    else if (userLimit_ and members_.size() at_least userLimit_)    err = ERR_LIMIT;
    else if (inviteOnly_ and not isInvited(nickname))               err = ERR_INVITE;
    else if (not key_.empty() and providedKey not_eq key_)          err = ERR_KEY;

    if (not err.empty()) { LOG_WARNING("@" + nickname + ": cannot join " + name_ + " - " + err);
                           if      (err eq ERR_MEMBER)    throw runtime_error(Replies::ERR_USERONCHANNEL(nickname,  name_));
                           else if (err eq ERR_LIMIT)     throw runtime_error(Replies::ERR_CHANNELISFULL(nickname,  name_));
                           else if (err eq ERR_INVITE)    throw runtime_error(Replies::ERR_INVITEONLYCHAN(nickname, name_));
                           else if (err eq ERR_KEY)       throw runtime_error(Replies::ERR_BADCHANNELKEY(nickname,  name_)); };

    members_[fd] = cli;
    LOG_INFO("@" + nickname + " joined " + name_);

    if (inviteOnly_ and isInvited(nickname))
        removeInvitation(nickname);

    if (operators_.empty()) { operators_[fd] = cli;
                              LOG_INFO("@" + nickname + " claimed operator role on " + name_);
    };
};