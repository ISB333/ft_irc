/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : srcs/Command/handleKick.cpp
** Author     : aheitz
** Created    : 2025-05-07
** Edited     : 2025-05-15
** Description: Managing the command to close the door on someone
*/

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

using namespace std;

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

void Handler::handleKick(Client  *client, const vector<std::string> &argv) {
    //TODO: Real implementation.
    Channel *channel = NULL;
    try                         { channel = server_.getChannel(argv[0]); }
    catch (const out_of_range&) { client->appendOutput(formatReply(403, client->getNickname(), argv[0], "No such channel"));
        return;
    };
    if (not channel->isOperator(client->getFd())) {
        client->appendOutput(formatReply(482, client->getNickname(), argv[0], "You're not channel operator"));
        return;
    };
};