/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : includes/Classes/Handler.hpp
** Author     : adesille, aheitz
** Created    : 2025-04-25
** Edited     : 2025-05-02
** Description: Every handler deserves a structure to track their dispatches
*/

#pragma  once

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

using namespace std;

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

class Server;

class Handler {
	public:
		Handler(Server& server);
		~Handler() {};
		void	dispatchCommand(Client* client, const std::string &message);

		void	handleNick(Client* client, const std::vector<std::string>& args);
		void	handleUser(Client* client, const std::vector<std::string>& args);
		void	handleJoin(Client* client, const std::vector<std::string>& args);
		void	handlePrivmsg(Client* client, const std::vector<std::string>& args);
		void	handlePassword(Client* client, const std::vector<std::string>& args);

		void	handleTopic(Client *client, const std::vector<std::string> &argv);
		void	handleMode(Client *client,  const std::vector<std::string> &argv);
	private:
		Server							&_server;
		typedef void (Handler::*CommandFunction)(Client*, const std::vector<std::string>&);
		map<string, CommandFunction>	commandMap;		
};
