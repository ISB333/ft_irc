/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : includes/Classes/Server.hpp
** Author     : adesille, aheitz
** Created    : 2025-04-23
** Edited     : 2025-04-25
** Description: Every channel deserves a structure to track their data
*/

#pragma once

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

using namespace std;

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

class Client;

class Channel {
	public:
		map<int, Client*>	members;
		map<int, Client*>	operators;
		string				name;
		string 				topic;
		string				key;
		bool				inviteOnly;
		bool				topicRestricted;
		int					userLimit;

		Channel(const std::string& name);
		~Channel() { };

		void				addClient(Client* client);
		void				removeClient(Client* client);
		void				setMode(char mode, bool set, const std::string& arg = "");

		bool				isMember(const int clientFd) const;
};