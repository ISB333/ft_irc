/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : includes/Classes/Channel/Channel.cpp
** Author     : adesille, aheitz
** Created    : 2025-04-23
** Edited     : 2025-05-02
** Description: Channel class member functions
*/

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

/**
 * @brief Construct a new Channel object
 * 
 * @param name Channel's name created
 */
Channel::Channel(const string &name) : name_(name),
                                       topic_(), key_(),  members_(), operators_(), invitedMembers_(),
                                       inviteOnly_(false), topicRestricted_(false),
                                       userLimit_(0) {};

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
 * @brief Getter to obtain the channel user limit
 * 
 * @return size_t The maximum number of users (0 if inactive)
 */
size_t Channel::getUserLimit(void) const { return userLimit_; };