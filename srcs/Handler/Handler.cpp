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

vector<string> splitMessage(const string &message) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(message);
    
    while (getline(tokenStream, token, ' ')) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    return tokens;
}

void Handler::dispatchCommand(Client* client, const string& message) {
    Command cmd = parseLine(message); 

    map<string, CommandFunction>::iterator it = commandMap_.find(cmd.name);
    if (it != commandMap_.end()) {
        (this->*(it->second))(client, cmd.argv);
    } else {
        client->sendReply(Replies::ERR_UNKNOWNCOMMAND(client->getNickname(), cmd.name));
    }
}
