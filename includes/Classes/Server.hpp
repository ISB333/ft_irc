/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : includes/Classes/Server.hpp
** Author     : adesille, aheitz
** Created    : 2025-04-23
** Edited     : 2025-05-12
** Description: Every server deserves a structure to track their data
*/

#pragma  once

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

class Server {
    public:
        Server(const int port, const std::string &password);
        ~Server(void);

        void    run(void);
        Channel *getChannel(const std::string &name) const;
        bool    isBanned(const std::string &ip)      const;
        void    ban(const std::string &ip);
        void    authenticate(Client *client, const std::string &password);

        void    saveServer(void) const;
        void    loadServer(void);

        void    addIPToBanList(const string IP) 	        {       bannedIPs_.insert(IP);       }
        void    addUser(const std::string &username)        { registeredUsers_.insert(username); }

        bool    isIPBanned(const string &IP)   {
            return (bannedIPs_.find(IP) != bannedIPs_.end());
        };
        bool    isNicknameAlreadyUsed(const string username) {
            return registeredUsers_.find(username) != registeredUsers_.end();
        };

    private:
        const int                      port_;
        const string               password_;
        int                          socket_;
        struct sockaddr_in          address_;
        auto_ptr<Handler>           handler_;
        vector<struct pollfd>       pollfds_;
        map<string, Channel*>      channels_;
        map<int, Client*>           clients_;
        set<string>                  banned_;
        set<string>               bannedIPs_;
        set<string>         registeredUsers_;

        void   setupSocket(void);
        void   onClientConnection(void);
        void   onClientReadable(const int fd);
        void   onClientWritable(const int fd);
        void   removeClient(const int fd, const std::string &reason);
        size_t disconnectInactives(void);
        void   removePollout(const int fd);

        Server(const Server &src);            // Non-instantiable
        Server &operator=(const Server &src); // Non-instantiable
};
