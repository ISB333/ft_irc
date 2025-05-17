/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : srcs/Replies.cpp
** Author     : adesille, aheitz
** Created    : 2025-05-16
** Edited     : 2025-05-16
** Description: All that damn data to respond to clients
*/

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

using namespace std;

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

string Replies::ERR_UNKNOWNCOMMAND(const string& nickname, const string& command) {
    return ":server 421 " + nickname + " " + command + " :Unknown command\r\n";
}

string Replies::ERR_ERRONEUSNICKNAME(const string& nickname) {
    return ":server 432 " + nickname + " :Erroneous nickname\r\n";
}

string Replies::ERR_NICKNAMEINUSE(const string& nickname) {
    return ":server 433 " + nickname + " :Nickname is already in use\r\n";
}

string Replies::ERR_ERRONEUSUSERNAME(const string& username) {
    return ":server 468 " + username + " :Erroneous username\r\n";
}

string Replies::ERR_ERRONEUSREALNAME(const string& realname) {
    return ":server 468 " + realname + " :Erroneous realname\r\n";
}

string Replies::ERR_PASSWDMISMATCH(const string& passwd, const int attempts) {
    return ":server 464 " + passwd + " :Password incorrect, " + intToString(3 - attempts) + " attempts left\r\n";
}

string Replies::ERR_YOUWILLBEBANNED() {
    return ":server 466 WARNING :You will be Banned\r\n";
}

string Replies::ERR_YOUREBANNEDCREEP() {
    return ":server 465 GOULAG CHIEF :You're now Banned for Life and it's too late to cry\r\n";
}

string Replies::ERR_UNKNOWNERROR(const string& target, const string& command, const string& message) {
    return ":server 400 " + target + " " + command + " :" + message + "\r\n";
}

string Replies::ERR_ALREADYBANNED() {
    return ":server 666 GTFO :You've already been Banned! Adios Boloss!\r\n";
}

string Replies::ERR_NOTREGISTERED(const string& command) {
    return ":server 451 " + command + " :You have not fully registered (PASS, NICK and USER necessary)\r\n";
}

string Replies::ERR_NEEDMOREPARAMETERS(const string& command) {
    return ":server 461 * " + command + " :Not enough parameters\r\n";
}

string Replies::ERR_ALREADYREGISTERED() {
    return ":server 462 * :You may not reregister\r\n";
}

// │──────────────────────────────────────────│ JOIN │──────────────────────────────────────────│ //

string Replies::JOIN(const string &prefix, const string &chan) {
    return ':' + prefix + " JOIN " + chan;
};

string Replies::RPL_NAMREPLY(const string &nick, const Channel *chan) {
    ostringstream stream;
    stream << ':' << SERVER_NAME << " 353 " << nick << " = " << chan->getName() << " :";

    const map<int, Client *> &members = chan->getMembers();
    map<int, Client *>::const_iterator  m       = members.begin();
        while (m not_eq members.end()) {
            if (chan->isOperator(m->first)) {
                stream << "@";
            };
            stream << m->second->getNickname();
            if (++m not_eq members.end()) {
                stream << " ";
            };
        };
    return stream.str();
};

string Replies::RPL_ENDOFNAMES(const string &nick, const string &chan) {
    return ':' + string(SERVER_NAME) + " 366 " + nick + ' ' + chan + " :End of NAMES list";
};

string Replies::ERR_USERONCHANNEL(const string &nick, const string &chan) {
    return ':' + string(SERVER_NAME) + " 443 " + nick + ' ' + chan + " :is already on channel";
};

string Replies::ERR_CHANNELISFULL(const string &nick, const string &chan) {
    return ':' + string(SERVER_NAME) + " 471 " + nick + ' ' + chan + " :Cannot join channel (+l)";
};

string Replies::ERR_INVITEONLYCHAN(const string &nick, const string &chan) {
    return ':' + string(SERVER_NAME) + " 473 " + nick + ' ' + chan + " :Cannot join channel (+i)";
};

string Replies::ERR_BADCHANNELKEY(const string &nick, const string &chan) {
    return ':' + string(SERVER_NAME) + " 475 " + nick + ' ' + chan + " :Cannot join channel (+k)";
};

string Replies::ERR_BADCHANMASK(const string &nick, const string &chan) {
    return ':' + string(SERVER_NAME) + " 476 " + nick + ' ' + chan + " :Bad Channel Mask";
};

// │──────────────────────────────────────────│ JOIN │──────────────────────────────────────────│ //

// │──────────────────────────────────────────│ TOPIC │─────────────────────────────────────────│ //

string Replies::TOPIC(const string &prefix, const string &chan, const string &topic) {
    return ':' + prefix + " TOPIC " + chan + " :" + topic;
};

string Replies::RPL_NOTOPIC(const string &nick, const string &chan) {
    return ':' + string(SERVER_NAME) + " 331 " + nick + ' ' + chan + " :No topic is set";
};

string Replies::RPL_TOPIC(const string &nick, const string &chan, const string &topic) {
    return ':' + string(SERVER_NAME) + " 332 " + nick + ' ' + chan + " :" + topic;
};

string Replies::ERR_NOSUCHCHANNEL(const string &nick, const string &chan) {
    return ':' + string(SERVER_NAME) + " 403 " + nick + ' ' + chan + " :No such channel";
};

string Replies::ERR_NOTONCHANNEL(const string &nick, const string &chan) {
    return ':' + string(SERVER_NAME) + " 442 " + nick + ' ' + chan + " :You're not on that channel";
};

string Replies::ERR_CHANOPRIVSNEEDED(const string &nick, const string &chan) {
    return ':' + string(SERVER_NAME) + " 482 " + nick + ' ' + chan + " :You're not channel operator";
};

// │──────────────────────────────────────────│ TOPIC │─────────────────────────────────────────│ //

// │─────────────────────────────────────────│ PRIVMSG │────────────────────────────────────────│ //

string Replies::ERR_NORECIPIENT(const string& command) {
    return "411 :No recipient given (" + command + ")";
};

string Replies::ERR_CANNOTSENDTOCHAN(const string& channel) {
    return "404 " + channel + " :Cannot send to channel";
};

string Replies::ERR_NOSUCHNICK(const string& nickname) {
    return "401 " + nickname + " :No such nick/channel";
};

string Replies::ERR_NOTEXTTOSEND() {
    return "412 :No text to send";
};

// │─────────────────────────────────────────│ PRIVMSG │────────────────────────────────────────│ //
