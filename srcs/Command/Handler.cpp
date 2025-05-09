/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : srcs/Command/Handler.cpp
** Author     : adesille, aheitz
** Created    : 2025-04-25
** Edited     : 2025-05-02
** Description: Handler class member functions
*/

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

using namespace std;

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

Handler::Handler(Server& server) : server_(server) {
	commandMap_["NICK"]    = &Handler::handleNick;
    commandMap_["USER"]    = &Handler::handleUser;
    commandMap_["JOIN"]    = &Handler::handleJoin;
    commandMap_["PRIVMSG"] = &Handler::handlePrivmsg;
    commandMap_["PASS"]    = &Handler::handlePassword;
    commandMap_["TOPIC"]   = &Handler::handleTopic;
    commandMap_["MODE"]    = &Handler::handleMode;
}

std::vector<std::string> splitMessage(const std::string &message) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(message);
    
    while (std::getline(tokenStream, token, ' ')) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    return tokens;
}

void Handler::dispatchCommand(Client* client, const std::string& message) {
    Command cmd = parseLine(message); 

    std::map<std::string, CommandFunction>::iterator it = commandMap_.find(cmd.name);
    if (it != commandMap_.end()) {
        (this->*(it->second))(client, cmd.argv);
    } else {
        client->sendReply(Replies::ERR_UNKNOWNCOMMAND(client->getNickname(), cmd.name));
    }
}

void	Handler::handlePassword(Client* client, const std::vector<std::string>& args) {
	std::cout << "ME Handle PASS" << std::endl;
	if (args.size() > 1)
	    client->sendReply(Replies::ERR_UNKNOWNERROR("*", "PASS", "Too many arguments"));
	else if (!args.empty() && !args[0].empty())
    	server_.authentification(client, args[0]);
}

bool isValidNickname(const std::string& nickname) {
    if (nickname.empty() || nickname.length() > 9 || std::isdigit(nickname[0]))
        return false;
    for (std::string::size_type i = 0; i < nickname.length(); ++i) {
        char c = nickname[i];
        if (!std::isalnum(c) && c != '[' && c != ']' &&
            c != '{' && c != '}' && c != '\\' && c != '|') {
            return false;
        }
    }
    return true;
}

void	Handler::handleNick(Client* client, const std::vector<std::string>& args) {
	if (!client->getAuthentication()) {
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
	std::cout << "ME Handle NICK" << std::endl;
}

bool isValidUsername(const std::string& username) {
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

bool isValidRealname(const std::string& realname) {
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
void	Handler::handleUser(Client* client, const std::vector<std::string>& args) {
	if (!client->getAuthentication() || client->getNickname().empty()) {
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
	std::cout << "ME Handle USER" << std::endl;
}

void	Handler::handleJoin(Client* client, const std::vector<std::string>& args) {
	if (!client->isFullyRegistered()) {
	   client->sendReply(Replies::ERR_NOTREGISTERED("JOIN"));
    	return;
	}
	if (args.empty() || args[0].empty()) {
		client->sendReply(Replies::ERR_NEEDMOREPARAMETERS("JOIN"));
		return;
	}

	(void)args;
	std::cout << "ME Handle JOIN" << std::endl;
}

void	Handler::handlePrivmsg(Client* client, const std::vector<std::string>& args) {
	if (!client->isFullyRegistered()) {
	    client->sendReply(Replies::ERR_NOTREGISTERED("PRIVMSG"));
    	return;
	}
	if (args.empty() || args[0].empty()) {
		client->sendReply(Replies::ERR_NEEDMOREPARAMETERS("PRIVMSG"));
		return;
	}

	(void)args;
	std::cout << "ME Handle PRIVMSG" << std::endl;
}
