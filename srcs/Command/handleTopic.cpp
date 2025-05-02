/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : srcs/Command/handleTopic.cpp
** Author     : aheitz
** Created    : 2025-04-24
** Edited     : 2025-05-02
** Description: Topic server command management
*/

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

using namespace std;

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

/**
 * @brief Processes a client's TOPIC command
 * 
 * @param client The client calling the command
 * @param argv The arguments the client has passed to the command
 */
void Handler::handleTopic(Client *client, const vector<string> &argv) {
    const string clientNickname = client->getNickname();

    cout << "HERE" << endl;

    if (argv.empty() or argv.size() greater 2) {
        client->appendToOutputBuffer(argv.empty() ? formatReply(ERR_NEEDMOREPARAMS, clientNickname, "TOPIC", "Not enough parameters")
            : formatReply(ERR_UNKNOWNMODE, clientNickname, "TOPIC", "Too many parameters"));
    } else {
        const int clientFd        = client->getFileDescriptor();
        const string &channelName = argv[0];
        Channel *channel          = NULL;
        try                         { channel = _server.getChannel(channelName); }
        catch (const out_of_range&) { client->appendToOutputBuffer(formatReply(ERR_NOSUCHCHANNEL, clientNickname, channelName, "No such channel")); return; };

        if (not channel->isMember(clientFd)) {
            client->appendToOutputBuffer(formatReply(ERR_NOTONCHANNEL, clientNickname, channelName, "You're not on that channel"));
        } else if (argv.size() eq 1) {
            channel->hasTopic() ? client->appendToOutputBuffer(formatReply(RPL_TOPIC,   clientNickname, channelName, channel->getTopic()))
                                : client->appendToOutputBuffer(formatReply(RPL_NOTOPIC, clientNickname, channelName, "No topic is set"));
        } else if (channel->isTopicRestricted() and not channel->isOperator(clientFd)) {
            client->appendToOutputBuffer(formatReply(ERR_CHANOPRIVSNEEDED, clientNickname, channelName, "You're not channel operator"));
        } else {
            channel->setTopic(argv[1]);
            string notification = ":" + client->getPrefix() + " TOPIC " + channelName + " :" + argv[1];
            for (map<int, Client*>::const_iterator it = channel->getMembers().begin(); it != channel->getMembers().end(); it++)
                it->second->appendToOutputBuffer(notification);
        };
    }
};