/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
<<<<<<<< HEAD:srcs/Handler/join.cpp
** File       : join.cpp
** Author     : adesille, aheitz
** Created    : 2025-05-12
** Edited     : 2025-05-12
** Description: 
========
** File       : srcs/Command/handleKick.cpp
** Author     : aheitz
** Created    : 2025-05-07
** Edited     : 2025-05-07
** Description: Managing the command to close the door on someone
>>>>>>>> main:srcs/Command/handleKick.cpp
*/

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

using namespace std;

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

void	Handler::handleJoin(Client* client, const vector<string>& args) {
	// if (!client->isFullyRegistered()) {
	if (true) { //FIXME: Fix this horrible thing.
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
