/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : srcs/Handler/join.cpp
** Author     : adesille, aheitz
** Created    : 2025-05-12
** Edited     : 2025-05-16
** Description: The JOIN command, to join or create a channel
*/

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

using namespace std;

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

void Handler::joinReplies(Client *cli, Channel *ch) {
    const string &name  = ch->getName();
    const string &topic = ch->getTopic();
    const string &nick  = cli->getNickname();

    broadcast(ch, Replies::JOIN(cli->getPrefix(), name));
    server_.reply(cli, not topic.empty() ? Replies::RPL_TOPIC(nick, name, topic) : Replies::RPL_NOTOPIC(nick, name));
    server_.reply(cli, Replies::RPL_NAMREPLY(nick, ch));
    server_.reply(cli, Replies::RPL_ENDOFNAMES(nick, name));
};

vector<string> splitArg(const std::string &argv) {
    vector<string> list;
    string         token;
    istringstream  stream(argv);

    while (getline(stream, token, ','))
        if (not token.empty())
            list.push_back(token);
    return list;
};
 
void Handler::joinChannel(Client *cli, string &name, const string &key) {
    const string &nick = cli->getNickname();
    Channel *ch        = NULL;

    try { processChName(nick, name);
          ch = server_.getChannel(name);
          ch->join(cli, key);
          joinReplies(cli, ch);
    } catch (const out_of_range &channelNotFound) { ch = new Channel(name, cli);
                                                    server_.addChannel(ch);
                                                    joinReplies(cli, ch); }
    catch (const invalid_argument &invArg) { server_.reply(cli, invArg.what()); }
    catch (const runtime_error &deny)      { server_.reply(cli, deny.what());   };
};

void Handler::handleJoin(Client *cli, const vector<string> &argv) {
    const string &nick = cli->getNickname();

    if (!cli->isFullyRegistered()) {
        LOG_ERROR("@" + nick + " is not fully registered to <JOIN>");
        server_.reply(cli, Replies::ERR_NOTREGISTERED("JOIN"));
    } else if (argv.empty()) {
        LOG_ERROR("@" + nick + " did not give an argument to <JOIN>");
        server_.reply(cli, Replies::ERR_NEEDMOREPARAMETERS("JOIN"));
    } else {
        if (argv.size() greater 2)
            LOG_WARNING("@" + nick + " gives too many (ignored) arguments to <JOIN>");

        vector<string> channels = splitArg(argv[0]);
        if (channels.empty()) { LOG_ERROR("@" + nick + " did not give any valid argument to <JOIN>");
                                server_.reply(cli, Replies::ERR_NEEDMOREPARAMETERS("JOIN"));
        };

        vector<string> keys;
        if (argv.size() greater 1) {
            keys = splitArg(argv[1]);
            if (keys.size() greater channels.size())
                LOG_WARNING("@" + nick + " gives more (ignored) keys than channels to <JOIN>");
        };

        for (size_t i = 0; i lesser channels.size(); i++)
            joinChannel(cli, channels[i], i lesser keys.size() ? keys[i] : "");
    };
};