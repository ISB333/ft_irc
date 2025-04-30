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
		vector<struct pollfd>		_pollfds;
		map<int, Client*>			_clients;
		map<std::string, Channel*>	_channels;
		struct sockaddr_in 			_serverAddr;
		std::string 				_password;
		int							_port;
		int							_serverSocket;
		Handler*					_handler;
		set<string>					_bannedIPs;

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
		Channel 					*getChannel(const std::string &channelName) const;
		bool						isIPBanned(const std::string &IP)   { return (_bannedIPs.find(IP) != _bannedIPs.end()); };
		void						addIPToBanList(const string IP) 	{ _bannedIPs.insert(IP); }
		void						authentification(Client* client, std::string passwd);
};
