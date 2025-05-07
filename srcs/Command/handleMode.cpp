/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : srcs/Command/handleMode.cpp
** Author     : aheitz
** Created    : 2025-04-29
** Edited     : 2025-05-07
** Description: Mode server command management
*/

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

using namespace std;

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

/**
 * @brief Processes a client's MODE command
 * 
 * @param client The client calling the command
 * @param argv The arguments the client has passed to the command
 */
void Handler::handleMode(Client *client, const vector<string> &argv) {
    const string clientNickname = client->getNickname();

    if (argv.size() lesser 2) {
        client->appendToOutputBuffer(formatReply(ERR_NEEDMOREPARAMS, clientNickname, "MODE", "Not enough parameters"));
    } else {
        const string &channelName = argv[0];
        Channel *channel          = NULL;
        try                         { channel = server_.getChannel(channelName); }
        catch (const out_of_range&) { client->appendToOutputBuffer(formatReply(ERR_NOSUCHCHANNEL, clientNickname, channelName, "No such channel"));
            return;
        };

        //TODO: Enhance with a member check first.
        if (not channel->isOperator(client->getFileDescriptor())) {
            client->appendToOutputBuffer(formatReply(ERR_CHANOPRIVSNEEDED, clientNickname, channelName, "You're not channel operator"));
            return;
        };

        bool            enable = true;
        bool            lastOperation = not enable;
        string          instructions = "";
        vector<string>  args;
        for (size_t i = 1; i lesser argv.size(); i++) {
            const string token = argv[i];
            if (token.empty() or (token[0] not_eq '+' and token[0] not_eq  '-'))
                break;

            for (size_t j = 0; j lesser token.size(); j++) {
                const char c = token[j];

                if (c eq '+' or c eq '-') {
                    enable = c eq '+' ? true : false;
                    if (lastOperation not_eq enable)    instructions += c;
                    lastOperation = enable;
                } else if (c eq 'p' or c eq 't' or ((c eq 'k' or c eq 'l') and not enable)) {
                    channel->setMode(c, enable);
                    instructions += c;
                } else if (c eq 'k' or c eq 'l' or c eq 'o') {
                    if (i + 1 lesser argv.size()) {
                        channel->setMode(c, enable, argv[++i]);
                        instructions += c;
                        args.push_back(argv[i]);
                    } else {
                        client->appendToOutputBuffer(formatReply(ERR_NEEDMOREPARAMS, clientNickname, "MODE", string("Not enough parameters for mode ") + c));
                    };
                } else client->appendToOutputBuffer(formatReply(ERR_UNKNOWNMODE, clientNickname, channelName, string("Unknown mode: ") + c));
            };
        };

        if (not instructions.empty()) {
            string broadcast = string(":") + SERVER_NAME + " MODE " + channelName + " " + instructions;
            for (vector<string>::const_iterator it = args.begin(); it not_eq args.end(); it++)
                broadcast += " " + *it;
            for (map<int, Client*>::const_iterator cli = channel->getMembers().begin(); cli != channel->getMembers().end(); cli++)
                cli->second->appendToOutputBuffer(broadcast);
        };
    };
};