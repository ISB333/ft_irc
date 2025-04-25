/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adesille <adesille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 10:06:41 by adesille          #+#    #+#             */
/*   Updated: 2025/04/25 11:11:23 by adesille         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "ircServ.hpp"

Handler::Handler(Server& server) : server(server) {
	commandMap["NICK"] = &Handler::handleNick;
    commandMap["USER"] = &Handler::handleUser;
    commandMap["JOIN"] = &Handler::handleJoin;
    commandMap["PRIVMSG"] = &Handler::handlePrivmsg;
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

    std::map<std::string, CommandFunction>::iterator it = commandMap.find(cmd.name);
    if (it != commandMap.end()) {
        (this->*(it->second))(client, cmd.argv);
    } else {
        client->sendReply(Replies::ERR_UNKNOWNCOMMAND(client->nickname, cmd.name));
    }
}

void	Handler::handleNick(Client* client, const std::vector<std::string>& args) {
	std::cout << "ME Handle NICK" << std::endl;
}
void	Handler::handleUser(Client* client, const std::vector<std::string>& args) {
	std::cout << "ME Handle USER" << std::endl;
}
void	Handler::handleJoin(Client* client, const std::vector<std::string>& args) {
	std::cout << "ME Handle JOIN" << std::endl;
}
void	Handler::handlePrivmsg(Client* client, const std::vector<std::string>& args) {
	std::cout << "ME Handle PRIVMSG" << std::endl;
}
