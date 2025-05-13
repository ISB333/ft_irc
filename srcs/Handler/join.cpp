/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : join.cpp
** Author     : adesille, aheitz
** Created    : 2025-05-12
** Edited     : 2025-05-12
** Description: 
*/

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

using namespace std;

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

void	Handler::handleJoin(Client* client, const vector<string>& args) {
	if (!client->isFullyRegistered()) {
	   client->sendReply(Replies::ERR_NOTREGISTERED("JOIN"));
    	return;
	}
	if (args.empty() || args[0].empty()) {
		client->sendReply(Replies::ERR_NEEDMOREPARAMETERS("JOIN"));
		return;
	}

	(void)args;
	cout << "ME Handle JOIN" << endl;
}
