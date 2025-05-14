/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : includes/Classes/Client.hpp
** Author     : adesille, aheitz
** Created    : 2025-04-22
** Edited     : 2025-05-13
** Description: Every client deserves a structure to track their connection
*/

//TODO: Check the relevance of each member function.

#pragma  once

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

class Client {
    public:
        explicit Client(const int fd);
        ~Client(void);

        void   appendInput(const char input[], const size_t size);
        string popInput(void);

        void appendOutput(const string &message);
        void consumeOutput(const size_t n);

        int          getFd(void)           const;
        int          getActivity(void)     const;
        bool         isAuthenticated(void) const;
        bool         isPassAuth(void)      const;
        int          getAttempt(void)      const;
        const string &getUsername(void)    const;
        const string &getNickname(void)    const;
        const string &getIp(void)          const;
        const string getPrefix(void)       const;
        const string &getInput(void)       const;
        const string &getOutput(void)      const;

        void authenticate(const bool change);
        void authenticatePass(const bool change);
        void setUsername(const std::string &username);
        void setRealname(const std::string realname);
        void setNickname(const std::string &nickname);
        void incrementAttempt(void);
        void updateActivity(void);
        
        bool isFullyRegistered(void) const;

    private:
        const int    fd_;
        bool       auth_;
        bool   authPass_;
		int     attempt_;
        string       ip_;
        string username_;
        string realname_;
        string nickname_;
        string    input_;
        string   output_;
        time_t activity_;

        Client(const Client &source);            // Non-instantiable
        Client &operator=(const Client &source); // Non-instantiable
};
