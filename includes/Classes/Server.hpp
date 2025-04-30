/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : includes/Classes/Server.hpp
** Author     : adesille, aheitz
** Created    : 2025-04-23
** Edited     : 2025-04-24
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
		vector<struct pollfd>		pollfds;
		map<int, Client*>			clients;
		map<std::string, Channel*>	_channels;
		struct sockaddr_in 			serverAddr;
		std::string 				password;
		int							port;
		int							serverSocket;
		Handler*					handler;

		void						setupSocket();
		void						handleNewConnection();
		void						handleClientData(int index);
		void						processCommand(Client* client, const std::string& message);
		void						removeClient(int fd);
		void						broadcastMessage(const std::string& message, Client* sender, Channel* channel = NULL);

	public:
		Server(int port, const std::string& password);
		~Server();

		void						run();
		Channel *getChannel(const std::string &channelName) const;
		void						authentification(std::string passwd);
};
