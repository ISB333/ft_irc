/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : privMSG.cpp
** Author     : adesille, aheitz
** Created    : 2025-05-12
** Edited     : 2025-05-12
** Description: 
*/

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

using namespace std;

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

void	Handler::handlePrivmsg(Client* client, const vector<string>& args) {
	// if (!client->isFullyRegistered()) {
	if (true) { //FIXME: Fix this horrible thing.
	    client->sendReply(Replies::ERR_NOTREGISTERED("PRIVMSG"));
    	return;
	}
	if (args.empty() || args[0].empty()) {
		client->sendReply(Replies::ERR_NEEDMOREPARAMETERS("PRIVMSG"));
		return;
	}

	(void)args;
	cout << "ME Handle PRIVMSG" << endl;
}
