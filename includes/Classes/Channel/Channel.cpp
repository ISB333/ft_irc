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
    } else LOG_INFO("Loading " + name + "...");
};

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

/**
 * @brief Getter for the channel name
 * 
 * @return const string& Simply the channel name
 */
const string &Channel::getName(void) const { return name_; };

void Channel::setTopic(const string &name) {
    LOG_DEBUG("\"" + name + "\" is the new topic of " + name_);
    topic_ = name;
};

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

void Channel::setKey(const string &key) {
    LOG_DEBUG("Key defined for " + name_);
    key_ = key;
};

void Channel::setInviteOnly(const bool enable) {
    LOG_DEBUG("Invite mode defined \"" + string(enable ? "true" : "false") + "\" for " + name_);
    inviteOnly_ = enable;
};

void Channel::setTopicRestricted(const bool enable) {
    LOG_DEBUG("Restricted mode defined \"" + string(enable ? "true" : "false") + "\" for " + name_);
    topicRestricted_ = enable;
};

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

void Channel::setUserLimit(const size_t limit) {
    if (limit)    LOG_DEBUG(name_ + " is now limited to " + intToString(limit) + " users");
    else          LOG_DEBUG(name_ + " is now open to unlimited users");
    userLimit_ = limit;
};

/**
 * @brief Getter to obtain the channel user limit
 * 
 * @return size_t The maximum number of users (0 if inactive)
 */
size_t Channel::getUserLimit(void) const { return userLimit_; };