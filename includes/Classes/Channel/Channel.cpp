/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : includes/Classes/Channel/Channel.cpp
** Author     : adesille, aheitz
** Created    : 2025-04-23
** Edited     : 2025-05-16
** Description: Channel class member functions
*/

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

Channel::Channel(const string &name, Client *owner) : name_(name),
                                       topic_(), key_(),  members_(), operators_(), invitedMembers_(),
                                       inviteOnly_(false), topicRestricted_(false),
                                       userLimit_(0)  {
    if (owner) { const int  fd  = owner->getFd();
                 members_  [fd] = owner;
                 operators_[fd] = owner;
                 LOG_INFO(name + " created by @" + owner->getNickname());
    } else LOG_DEBUG(name + " loaded");
};

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

/**
 * @brief Getter for the channel name
 * 
 * @return const string& Simply the channel name
 */
const string &Channel::getName(void) const { return name_; };

/**
 * @brief Define a new channel topic
 * 
 * @param name Topic name
 */
void Channel::setTopic(const string &name) { topic_ = name; };

/**
 * @brief Confirms whether the channel already has a subject
 * 
 * @return true If so
 * @return false Otherwise
 */
bool Channel::hasTopic(void) const { return not topic_.empty(); };

/**
 * @brief Getter for the channel topic
 * 
 * @return const string& Simply the channel topic
 */
const string &Channel::getTopic(void) const { return topic_; };

/**
 * @brief Getter for the channel key
 * 
 * @return const string& Simply the channel key
 */
const string &Channel::getKey(void) const { return key_; };

/**
 * @brief Define a new key for the channel
 * 
 * @param key Keyword
 */
void Channel::setKey(const string &key) { key_ = key; };

/**
 * @brief Enable (or not) invite mode for the channel
 * 
 * @param enable Activation (or non-activation) of the mode
 */
void Channel::setInviteOnly(const bool enable) { inviteOnly_ = enable; };

/**
 * @brief Enable (or not) restricted topic mode for the channel
 * 
 * @param enable Activation (or non-activation) of the mode
 */
void Channel::setTopicRestricted(const bool enable) { topicRestricted_ = enable; } ;

/**
 * @brief Getter for channel invite mode
 * 
 * @return true If mode is enabled
 * @return false Otherwise
 */
bool Channel::isInviteOnly(void) const { return inviteOnly_; };

/**
 * @brief Getter for channel topic restriction
 * 
 * @return true If restriction is set
 * @return false Otherwise
 */
bool Channel::isTopicRestricted(void) const { return topicRestricted_; };

/**
 * @brief Defines a limit on the number of users on the channel
 *
 * @param limit The limit value
 */
void Channel::setUserLimit(const size_t limit) { userLimit_ = limit; };

/**
 * @brief Getter to obtain the channel user limit
 * 
 * @return size_t The maximum number of users (0 if inactive)
 */
size_t Channel::getUserLimit(void) const { return userLimit_; };