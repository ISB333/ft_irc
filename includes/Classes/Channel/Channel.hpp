/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : includes/Classes/Channel/Channel.hpp
** Author     : adesille, aheitz
** Created    : 2025-04-23
** Edited     : 2025-04-28
** Description: Every channel deserves a structure to track their data
*/

#pragma once

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

//TODO: Check the usefulness of each function at the end of the project.

/**
 * @brief A class that manages an entire channel, its parameters and users
 * 
 */
class Channel {
    public:
        explicit Channel(const std::string& name);

        const std::string &getName(void)          const;
        const std::string &getTopic(void)         const;
        bool              isInviteOnly(void)      const;
        bool              isTopicRestricted(void) const;
        size_t            getUserLimit(void)      const;
        
        void tryJoin(Client *client, const std::string &providedKey = "");
        bool hasTopic(void) const;

        void                         addClient(Client* client);
        void                         removeClient(const int clientFd);
        bool                         isMember(const int clientFd)       const;
        bool                         isOperator(const int clientFd)     const;
        const std::map<int, Client*> &getMembers(void)                  const;
        const std::map<int, Client*> &getOperators(void)                const;
        Client                       *getClient(const int clientFd)     const;

        void                inviteClient(const int clientFd);
        void                removeInvitation(const int clientFd);
        bool                isInvited(const int clientFd)         const;
        const std::set<int> &getInvitedMembers(void)              const;

        void setMode(const char type, const bool set = true, const std::string &context = "");
    private:
        const std::string      _name;
        std::string            _topic;
        std::string            _key;
        std::map<int, Client*> _members;
        std::map<int, Client*> _operators;
        std::set<int>          _invitedMembers;
        bool                   _inviteOnly;
        bool                   _topicRestricted;
        size_t                 _userLimit;
};