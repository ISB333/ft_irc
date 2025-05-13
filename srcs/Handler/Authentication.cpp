/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : Authentification.cpp
** Author     : adesille, aheitz
** Created    : 2025-05-12
** Edited     : 2025-05-12
** Description: manage user registration 
*/

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

using namespace std;

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

void	Handler::handlePassword(Client* client, const vector<string>& args) {
	cout << "ME Handle PASS" << endl;
	if (args.size() > 1)
	    client->sendReply(Replies::ERR_UNKNOWNERROR("*", "PASS", "Too many arguments"));
	else if (!args.empty() && !args[0].empty())
    	server_.authenticate(client, args[0]);
}

bool isValidNickname(const string& nickname) {
    if (nickname.empty() || nickname.length() > 9 || isdigit(nickname[0]))
        return false;
    for (string::size_type i = 0; i < nickname.length(); ++i) {
        char c = nickname[i];
        if (!isalnum(c) && c != '[' && c != ']' &&
            c != '{' && c != '}' && c != '\\' && c != '|') {
            return false;
        }
    }
    return true;
}

void	Handler::handleNick(Client* client, const vector<string>& args) {
	if (!client->isAuthenticated()) {
	    client->sendReply(Replies::ERR_NOTREGISTERED("NICK"));
    	return;
	}
	if (args.empty() || args[1].empty()) {
		client->sendReply(Replies::ERR_NEEDMOREPARAMETERS("NICK"));
		return;
	}
	if (server_.isNicknameAlreadyUsed(args[0])) {
	    client->sendReply(Replies::ERR_NICKNAMEINUSE(args[0]));
    	return;
	}
	else {
		if (isValidNickname(args[0])) {
			server_.addUser(args[0]);
			client->setNickname(args[0]);
		}
		else
			client->sendReply(Replies::ERR_ERRONEUSNICKNAME(args[0]));
	}
	cout << "ME Handle NICK" << endl;
}

bool isValidUsername(const string& username) {
	if (username.empty())
		return false;
    for (size_t i = 0; i < username.length(); ++i) {
        char c = username[i];
        if (!isalnum(c) && c != '_' && c != '-' && c != '.') {
            return false;
        }
    }
    return true;
}

bool isValidRealname(const string& realname) {
	if (realname.empty())
		return false;
    for (size_t i = 0; i < realname.length(); ++i) {
        unsigned char c = realname[i];
        if (c < 32 || c > 126) {
            return false;
        }
    }
    return true;
}

// TODO: Should register the Mode of the User(?)
void	Handler::handleUser(Client* client, const vector<string>& args) {
	if (!client->isAuthenticated() || client->getNickname().empty()) {
	    client->sendReply(Replies::ERR_NOTREGISTERED("USER"));
    	return;
	}
	if (args.empty() || args.size() < 5) {
		client->sendReply(Replies::ERR_NEEDMOREPARAMETERS("USER"));
		return;
	}
	else {
		if (isValidUsername(args[1])) {
			if (isValidRealname(args[4])) {
				client->sendReply(Replies::ERR_ERRONEUSREALNAME(args[4]));
				return;
			}
			client->setUsername(args[1]);
			client->setRealname(args[4]);
		}
		else {
			client->sendReply(Replies::ERR_ERRONEUSUSERNAME(args[0]));
			return;
		}
	}
	cout << "ME Handle USER" << endl;
}
