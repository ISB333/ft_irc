/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : includes/Classes/Server.hpp
** Author     : adesille, aheitz
** Created    : 2025-04-23
** Edited     : 2025-05-06
** Description: Every server deserves a structure to track their data
*/

#pragma  once

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

using namespace std;

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

class Handler;

class Server {
    private:
        vector<struct pollfd>           pollfds_;
        map<int, Client*>               clients_;
        map<std::string, Channel*>     channels_;
        struct sockaddr_in           serverAddr_;
        std::string                    password_;
        int                                port_;
        int                        serverSocket_;
        Handler*                        handler_;
        set<string>                   bannedIPs_;

        void    setupSocket();
        void    handleNewConnection();
        void    handleClientData(int index);
        void    processCommand(Client* client, const std::string& message);
        void    removeClient(int fd);
        void    broadcastMessage(const std::string& message, Client* sender, Channel* channel = NULL);

    public:
        Server(int port, const std::string& password);
        ~Server();

        void    run();
        Channel *getChannel(const std::string &channelName) const;
        bool    isIPBanned(const std::string &IP)   { return (bannedIPs_.find(IP) != bannedIPs_.end()); };
        void    addIPToBanList(const string IP) 	{ bannedIPs_.insert(IP); }
        void    authentification(Client* client, std::string passwd);

        void    saveServerChannels(void) const;
        void    loadServerChannels(void);
};
