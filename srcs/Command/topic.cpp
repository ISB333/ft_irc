/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : srcs/Command/topic.cpp
** Author     : aheitz
** Created    : 2025-04-24
** Edited     : 2025-04-29
** Description: Topic server command management
*/

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

using namespace std;

#define RPL_NOTOPIC          331
#define RPL_TOPIC            332
#define ERR_NOSUCHCHANNEL    403
#define ERR_NOTONCHANNEL     442
#define ERR_NEEDMOREPARAMS   461
#define ERR_UNKNOWNMODE      472
#define ERR_CHANOPRIVSNEEDED 482

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

/**
 * @brief Processes a client's TOPIC command
 * 
 * @param client The client calling the command
 * @param argv The arguments the client has passed to the command
 */
void Handler::handleTopic(Client &client, const vector<string> &argv) {
    const string clientNickname = client.getNickname();

    if (argv.empty() or argv.size() greater 2) {
        client.appendToOutputBuffer(argv.empty() ? formatReply(ERR_NEEDMOREPARAMS, clientNickname, "TOPIC", "Not enough parameters")
            : formatReply(ERR_UNKNOWNMODE, clientNickname, "TOPIC", "Too many parameters"));
    } else {
        const int clientFd        = client.getFileDescriptor();
        const string &channelName = argv[0];
        Channel *channel          = NULL;
        try                         { channel = _server.getChannel(channelName); }
        catch (const out_of_range&) { client.appendToOutputBuffer(formatReply(ERR_NOSUCHCHANNEL, clientNickname, channelName, "No such channel")); };

        if (not channel->isMember(clientFd)) {
            client.appendToOutputBuffer(formatReply(ERR_NOTONCHANNEL, clientNickname, channelName, "You're not on that channel"));
        } else if (argv.size() eq 1) {
            channel->hasTopic() ? client.appendToOutputBuffer(formatReply(RPL_TOPIC,   clientNickname, channelName, channel->getTopic()))
                                : client.appendToOutputBuffer(formatReply(RPL_NOTOPIC, clientNickname, channelName, "No topic is set"));
        } else if (channel->isTopicRestricted() and not channel->isOperator(clientFd)) {
            client.appendToOutputBuffer(formatReply(ERR_CHANOPRIVSNEEDED, clientNickname, channelName, "You're not channel operator"));
        } else {
            channel->setTopic(argv[1]);
            string notification = ":" + client.getPrefix() + " TOPIC " + channelName + " :" + argv[1];
            for (map<int, Client*>::const_iterator it = channel->getMembers().begin(); it != channel->getMembers().end(); it++)
                it->second->appendToOutputBuffer(notification);
        };
    }
};