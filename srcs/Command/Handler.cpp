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

/* TODO: for any command 
	if (!client->isFullyRegistered()) {
	    client->sendReply(Replies::ERR_NOTREGISTERED(command));
    	return;
	}
*/

void	Handler::handleNick(Client* client, const std::vector<std::string>& args) {
	if (!client->isFullyRegistered()) {
	    client->sendReply(Replies::ERR_NOTREGISTERED(args[0]));
    	return;
	}

	std::cout << "ME Handle NICK" << std::endl;
}

void	Handler::handleUser(Client* client, const std::vector<std::string>& args) {
	if (!client->isFullyRegistered()) {
	    client->sendReply(Replies::ERR_NOTREGISTERED(args[0]));
    	return;
	}
	std::cout << "ME Handle USER" << std::endl;
}

void	Handler::handleJoin(Client* client, const std::vector<std::string>& args) {
	if (!client->isFullyRegistered()) {
	    client->sendReply(Replies::ERR_NOTREGISTERED(args[0]));
    	return;
	}
	std::cout << "ME Handle JOIN" << std::endl;
}

void	Handler::handlePrivmsg(Client* client, const std::vector<std::string>& args) {
	if (!client->isFullyRegistered()) {
	    client->sendReply(Replies::ERR_NOTREGISTERED(args[0]));
    	return;
	}
	std::cout << "ME Handle PRIVMSG" << std::endl;
}
