/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : includes/Classes/Handler.hpp
** Author     : adesille, aheitz
** Created    : 2025-04-25
** Edited     : 2025-05-15
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

		void	handleKick(Client  *client, const std::vector<std::string> &argv);
		void	handleTopic(Client *client, const std::vector<std::string> &argv);
		void	handleMode(Client  *client, const std::vector<std::string> &argv);

		void joinChannel(Client *cli, string &name, const string &key = "");
		void joinReplies(Client *cli, Channel *ch);

		void broadcast(Channel *ch, const std::string &msg) const {
			const map<int, Client *> members = ch->getMembers();

        	LOG_DEBUG("Broadcast to " + intToString(members.size()) + " members: " + msg);
            for (map<int, Client *>::const_iterator m = members.begin(); m not_eq members.end(); m++)
                server_.reply(m->second, msg);
        };

		void processChName(const string &cli, string &ch) const;
	private:
		Server							&server_;
		typedef void (Handler::*CommandFunction)(Client*, const std::vector<std::string>&);
		map<string, CommandFunction>	commandMap_;		
};
