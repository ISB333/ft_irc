/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : includes/Classes/Channel/inviteMode.cpp
** Author     : aheitz
** Created    : 2025-04-28
** Edited     : 2025-05-16
** Description: Functions used in invite mode
*/

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

/**
 * @brief Invites a client on the channel
 * 
 * @param clientFd The client to invite
 */
 void Channel::inviteClient(const string nickname) {
    if (invitedMembers_.find(nickname) eq invitedMembers_.end()) invitedMembers_.insert(nickname);
};

/**
 * @brief Removes a client from the invitation list
 * 
 * @param clientFd The client to be removed
 */
void Channel::removeInvitation(const string nickname) { invitedMembers_.erase(nickname); };

/**
 * @brief Getter to find out if a client is invited
 * 
 * @param clientFd The client to check
 * @return true If invited
 * @return false Otherwise
 */
bool Channel::isInvited(const string nickname) const { return invitedMembers_.count(nickname); };

/**
 * @brief Getter to obtain the invitation list
 * 
 * @return set<int> The complete invitation list
 */
const set<string> &Channel::getInvitedMembers(void) const { return invitedMembers_; };