/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : includes/Classes/Channel/setMode.cpp
** Author     : aheitz
** Created    : 2025-04-28
** Edited     : 2025-05-06
** Description: Declaration of function to safely change channel configurations
*/

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

/**
 * @brief Checks the values received in the operation and throws an exception if invalid
 * 
 * @param type The operation identifier
 * @param value The value transmitted for this operation
 */
 static void catchInvalidValue(const char type, const char *value) {
    errno                   = 0;
    char               *end = NULL;
    long               operatorConversion;
    unsigned long long limitConversion;

    type eq 'o' ? operatorConversion = strtol(value,   &end, 10)
                : limitConversion    = strtoull(value, &end, 10);
    
    if (*end)
        throw invalid_argument("MODE " + string(1, static_cast<char>(toupper(type))) + ": Invalid argument: " + value);

    if (errno == ERANGE or type eq 'o' ? operatorConversion lesser 0 or operatorConversion greater numeric_limits<int>::max()
                                       :                                limitConversion    greater numeric_limits<size_t>::max())
        throw out_of_range("MODE " + string(1, static_cast<char>(toupper(type))) + ": Logical overreach: " + value);
};

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

/**
 * @brief Updates the channel parameters according to the mode to be redefined. Throws an exception on invalid context
 * 
 * @param type The instruction to operate
 * @param set The modification direction
 * @param context A passable context for parameter modes
 */
 void Channel::setMode(const char type, const bool set, const string &context) {
    switch (type) {
        case 'k': setKey(context); break;
        case 'i': setInviteOnly(set); break;
        case 't': setTopicRestricted(set); break;
        case 'o': {
            catchInvalidValue('o', context.c_str());
            int clientFd = atoi(context.c_str());
            if (not isMember(clientFd))
                throw runtime_error("The client is not a channel member");
            if (set) promoteOperator(clientFd);
            else     operators_.erase(clientFd);
            break;
        };
        case 'l': {
            setUserLimit(set ? catchInvalidValue('l', context.c_str()), atoi(context.c_str()) : 0);
            break;
        };
        default: throw invalid_argument("MODE " + string(1, type) + ": unknown mode");
    };
};