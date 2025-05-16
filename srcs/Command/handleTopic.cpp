/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : srcs/Command/handleTopic.cpp
** Author     : aheitz
** Created    : 2025-04-24
** Edited     : 2025-05-16
** Description: Topic server command management
*/

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

using namespace std;

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

//TODO: Canal name MUST be checked!

void Handler::handleTopic(Client *cli, const vector<string> &argv) {
    const string nick = cli->getNickname();

    if (argv.empty()) {
        const string &reply = Replies::ERR_NEEDMOREPARAMETERS("TOPIC");
        LOG_ERROR(reply);
        server_.reply(cli, reply);
        return;
    };
    
    const int fd = cli->getFd();
    const string &name = argv[0];
    Channel *ch        = NULL;

    try {
        ch = server_.getChannel(name);
    } catch (const out_of_range&) {
        const string &reply = Replies::ERR_NOSUCHCHANNEL(nick, name);
        LOG_ERROR(reply);
        server_.reply(cli, reply);
        return;
    };

    if (not ch->isMember(fd)) {
        const string &reply = Replies::ERR_NOTONCHANNEL(nick, name);
        LOG_WARNING(reply);
        server_.reply(cli, reply);
        return;
    };
    
    if (argv.size() eq 1) {
        const std::string &topic = ch->getTopic();
        not topic.empty() ? server_.reply(cli, Replies::RPL_TOPIC(nick,   name, topic))
                          : server_.reply(cli, Replies::RPL_NOTOPIC(nick, name));
        LOG_DEBUG("@" + nick + " get `" + (not topic.empty() ? topic : "nothing") + "` from <TOPIC>");
        return;
    };

    string topic = argv[1];
    if (ch->isOperator(fd) or not ch->isTopicRestricted()) {
        for (size_t i = 2; i lesser argv.size() and topic.size() at_most IRC_LIMIT; i++) {
            topic += " " + argv[i];
        };

        if (topic.size() greater IRC_LIMIT) {
            LOG_ERROR("@" + nick + " get kicked from server for violating the send size in <TOPIC>");
            server_.removeClient(fd, "ERROR :Closing Link: line too long");
            return;
        };
        ch->setTopic(topic);

        const string &reply = Replies::TOPIC(cli->getPrefix(), name, topic);
        LOG_INFO(reply);
        broadcast(ch, reply);
    } else {
        const string &reply = Replies::ERR_CHANOPRIVSNEEDED(nick, name);
        LOG_WARNING(reply);
        server_.reply(cli, reply);
    };
};