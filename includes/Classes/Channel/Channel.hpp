/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : includes/Classes/Channel/Channel.hpp
** Author     : adesille, aheitz
** Created    : 2025-04-23
** Edited     : 2025-05-16
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
        explicit Channel(const std::string& name, Client *owner);

        const std::string &getName(void)          const;
        const std::string &getTopic(void)         const;
        const std::string &getKey(void)           const;
        bool              isInviteOnly(void)      const;
        bool              isTopicRestricted(void) const;

        void   setUserLimit(const size_t limit);
        size_t getUserLimit(void)                const;

        void setKey(const std::string &key);
        void join(Client *client, const std::string &providedKey = "");

        void setTopic(const std::string &name);
        bool hasTopic(void) const;

        void setInviteOnly(const bool enable);
        void setTopicRestricted(const bool enable);

        void                         addClient(Client* client);
        void                         removeClient(const int clientFd);
        bool                         isMember(const int clientFd)         const;
        void                         promoteOperator(const int clientFd);
        bool                         isOperator(const int clientFd)       const;
        const std::map<int, Client*> &getMembers(void)                    const;
        const std::map<int, Client*> &getOperators(void)                  const;
        Client                       *getClient(const int clientFd)       const;

        void                inviteClient(const string nickname);
        void                removeInvitation(const string nickname);
        bool                isInvited(const string nickname)      const;
        const std::set<string> &getInvitedMembers(void)              const;

        void setMode(const char type, const bool set = true, const std::string &context = "");
    private:
        const std::string                 name_;
        std::string                      topic_;
        std::string                        key_;
        std::map<int, Client*>         members_;
        std::map<int, Client*>       operators_;
        std::set<string>        invitedMembers_;
        bool                        inviteOnly_;
        bool                   topicRestricted_;
        size_t                       userLimit_;
};