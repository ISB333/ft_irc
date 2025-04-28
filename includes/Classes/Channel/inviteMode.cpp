/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : includes/Classes/Channel/inviteMode.cpp
** Author     : aheitz
** Created    : 2025-04-28
** Edited     : 2025-04-28
** Description: Functions used in invite mode
*/

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

/**
 * @brief Invites a client on the channel
 * 
 * @param clientFd The client to invite
 */
 void Channel::inviteClient(const int clientFd) {
    if (not isMember(clientFd)) _invitedMembers.insert(clientFd);
};

/**
 * @brief Removes a client from the invitation list
 * 
 * @param clientFd The client to be removed
 */
void Channel::removeInvitation(const int clientFd) { _invitedMembers.erase(clientFd); };

/**
 * @brief Getter to find out if a client is invited
 * 
 * @param clientFd The client to check
 * @return true If invited
 * @return false Otherwise
 */
bool Channel::isInvited(const int clientFd) const { return _invitedMembers.count(clientFd); };

/**
 * @brief Getter to obtain the invitation list
 * 
 * @return set<int> The complete invitation list
 */
const set<int> &Channel::getInvitedMembers(void) const { return _invitedMembers; };