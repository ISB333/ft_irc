/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : includes/Classes/Channel.cpp
** Author     : adesille, aheitz
** Created    : 2025-04-23
** Edited     : 2025-04-25
** Description: Channel class member functions
*/

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

#include "ircServ.hpp"

//TODO: Add documentation.
//TODO: Check functions in a second commit.

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

static void catchInvalidValue(const char type, const char *value);

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

Channel::Channel(const string &name) : _name(name),
                                       _topic(),           _key(), _members(), _operators(),
                                       _inviteOnly(false), _topicRestricted(false),
                                       _userLimit(0) {};

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

const string &Channel::getName(void)          const { return _name;            };
const string &Channel::getTopic(void)         const { return _topic;           };
bool         Channel::isInviteOnly(void)      const { return _inviteOnly;      };
bool         Channel::isTopicRestricted(void) const { return _topicRestricted; };
size_t       Channel::getUserLimit(void)      const { return _userLimit;       };

void Channel::tryJoin(const int clientFd, const bool isInvited, const string &providedKey) const {
    if      (isMember(clientFd))                                       throw runtime_error("Client already connected");
    else if (_userLimit       and _members.size() at_least _userLimit) throw runtime_error("Channel already full");
    else if (_inviteOnly      and not isInvited)                       throw runtime_error("Channel requires an invitation");
    else if (not _key.empty() and providedKey not_eq _key)             throw runtime_error("A valid password is required");
};

/**
 * @brief Confirms whether the channel already has a subject
 * 
 * @return true If so
 * @return false Otherwise
 */
bool Channel::hasTopic(void) const { return not _topic.empty(); };

void Channel::addClient(Client* client) { _members[client->getFileDescriptor()] = client; };

void Channel::removeClient(const int clientFd) {
    if (isOperator(clientFd))   _operators.erase(clientFd);
        _members.erase(clientFd);
};

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

const map<int, Client*> &Channel::getMembers(void)              const { return _members;   };
const map<int, Client*> &Channel::getOperators(void)            const { return _operators; };
Client                  *Channel::getClient(const int clientFd) const { return isMember(clientFd) ? _members.find(clientFd)->second : NULL; };

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

void Channel::setMode(const char type, const bool set, const string &context) {
    switch (type) {
        case 'k': set ? _key = context : _key = ""; break;
        case 'i': _inviteOnly      = set;           break;
        case 't': _topicRestricted = set;           break;
        case 'o': {
            catchInvalidValue('o', context.c_str()); //TODO: Manage upstream.
            int clientFd = atoi(context.c_str());
            if (not isMember(clientFd))
                throw runtime_error("The client is not a channel member");
            if (set) _operators[clientFd] = _members[clientFd];
            else     _operators.erase(clientFd);
        break; };
        case 'l': {
            catchInvalidValue('l', context.c_str()); //TODO: Manage upstream.
            set ? _userLimit = atoi(context.c_str())
                : _userLimit = 0;
        break; };
    };
};

static void catchInvalidValue(const char type, const char *value) {
    errno                   = 0;
    char               *end = NULL;
    long               operatorConversion;
    unsigned long long limitConversion;

    type eq 'o' ? operatorConversion = strtol(value,   &end, 10)
                : limitConversion    = strtoull(value, &end, 10);
    
    if (*end)
        throw invalid_argument("MODE " + string(1, static_cast<char>(toupper(type))) + ": Invalid argument: " + value);

    if (errno == ERANGE or type eq 'o' ? operatorConversion lesser 0 or operatorConversion greater static_cast<long>(numeric_limits<int>::max())
                                       :                                limitConversion    greater static_cast<unsigned long long>(numeric_limits<size_t>::max()))
        throw invalid_argument("MODE " + string(1, static_cast<char>(toupper(type))) + ": Logical overreach: " + value);
};