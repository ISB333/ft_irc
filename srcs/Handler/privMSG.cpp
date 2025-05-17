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
	if (!client->isFullyRegistered()) {
		server_.reply(client, Replies::ERR_NOTREGISTERED("PRIVMSG"));
    	return;
	}
	if (args.empty() || args[0].empty()) {
		server_.reply(client, Replies::ERR_NEEDMOREPARAMETERS("PRIVMSG"));
		return;
	}

	if (args.size() < 2 || args[1].empty()) {
        server_.reply(client, Replies::ERR_NOTEXTTOSEND());
        return;
    }

	string target = args[0];
    string message = args[1];
    
    // For targets with multiple recipients (separated by commas)
    // Example: user1,user2,#channel
    vector<string> recipients;
    // Split target by commas and add to recipients
    
    // Process each recipient
    // If target starts with '#', it's a channel
    // Otherwise, it's a user
	// For channel messages:

	if (target[0] == '#') {
		Channel* channel = server_.findChannel(target);
		if (!channel) {
			server_.reply(client, Replies::ERR_NOSUCHNICK(target));
			return;
		}
		
		// Check if user is in channel and has permission to speak
		if (!channel->isMember(client->getFd())) {
			server_.reply(client, Replies::ERR_CANNOTSENDTOCHAN(target));
			return;
		}
		
		// Send message to all channel members
		channel->broadcast(client->getPrefix() + " PRIVMSG " + target + " :" + message);
	} else {
		// For direct messages to users
		Client* targetClient = server_.findClient(target);
		if (!targetClient) {
			server_.reply(client, Replies::ERR_NOSUCHNICK(target));
			return;
		}
		
		// Send message to target user
		server_.reply(targetClient, client->getPrefix() + " PRIVMSG " + target + " :" + message);
	}
}
