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
		server_.reply(client, Replies::ERR_UNKNOWNERROR("*", "PASS", "Too many arguments"));
	else if (!args.empty() && !args[0].empty())
    	server_.passwdManager(client, args[0]);
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
	if (!client->isPassAuth()) {
		server_.reply(client, Replies::ERR_NOTREGISTERED("NICK"));
    	return;
	}
	else if (args.empty()) {
		server_.reply(client, Replies::ERR_NEEDMOREPARAMETERS("NICK"));
		return;
	}
	else if (server_.isNicknameAlreadyUsed(args[0])) {
		server_.reply(client, Replies::ERR_NICKNAMEINUSE(args[0]));
    	return;
	}
	else {
		if (isValidNickname(args[0])) {
			server_.addUser(args[0]);
			client->setNickname(args[0]);
		}
		else
			server_.reply(client, Replies::ERR_ERRONEUSNICKNAME(args[0]));
	}
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
    for (size_t i = 1; i < realname.length(); ++i) {
        unsigned char c = realname[i];
        if (c < ' ' || c > '~') {
            return false;
        }
    }
    return true;
}

// TODO: Should register the Mode of the User(?)
void	Handler::handleUser(Client* client, const vector<string>& args) {
	if (!client->isPassAuth() || client->getNickname().empty()) {
		server_.reply(client, Replies::ERR_NOTREGISTERED("USER"));
    	return;
	}
	else if (!client->getUsername().empty()) {
		server_.reply(client, Replies::ERR_ALREADYREGISTERED());
		return;
	}
	else if (args.empty() || args.size() < 4) {
		server_.reply(client, Replies::ERR_NEEDMOREPARAMETERS("USER"));
    	return;
	}
	else {
		if (isValidUsername(args[1])) {
			if (!isValidRealname(args[3])) {
				server_.reply(client, Replies::ERR_ERRONEUSREALNAME(args[3]));
				return;
			}
			client->setUsername(args[1]);
			client->setRealname(args[3]);
			client->authenticate(true);
		}
		else {
			server_.reply(client, Replies::ERR_ERRONEUSUSERNAME(args[0]));
			return;
		}
	}
}
