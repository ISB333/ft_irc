/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : includes/Classes/Replies.hpp
** Author     : adesille, aheitz
** Created    : 2025-04-25
** Edited     : 2025-05-16
** Description: TODO:
*/

#pragma  once

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

class Replies {
    public:
        static std::string ERR_UNKNOWNCOMMAND(const std::string& nickname, const std::string& command);
        static std::string ERR_ERRONEUSNICKNAME(const std::string& nickname);
        static std::string ERR_NICKNAMEINUSE(const std::string& nickname);
        static std::string ERR_ERRONEUSUSERNAME(const std::string& username);
        static std::string ERR_ERRONEUSREALNAME(const std::string& realname);
        static std::string ERR_PASSWDMISMATCH(const std::string& passwd, const int attempts);
        static std::string ERR_YOUWILLBEBANNED();
        static std::string ERR_YOUREBANNEDCREEP();
        static std::string ERR_UNKNOWNERROR(const std::string& target, const std::string& command, const std::string& message);
        static std::string ERR_ALREADYBANNED();
        static std::string ERR_NOTREGISTERED(const std::string& command);
        static std::string ERR_NEEDMOREPARAMETERS(const std::string& command);
        static std::string ERR_ALREADYREGISTERED();

        // │──────────────────────────────────────│ JOIN │──────────────────────────────────────│ //

        static std::string JOIN(const std::string &prefix, const std::string &chan);
        static std::string RPL_NAMREPLY(const std::string &nick, const Channel *chan);
        static std::string RPL_ENDOFNAMES(const std::string &nick, const std::string &chan);
        static std::string ERR_USERONCHANNEL(const std::string &nick, const std::string &chan); 
        static std::string ERR_CHANNELISFULL(const std::string &nick, const std::string &chan);
        static std::string ERR_INVITEONLYCHAN(const std::string &nick, const std::string &chan);
        static std::string ERR_BADCHANNELKEY(const std::string &nick, const std::string &chan);
        static std::string ERR_BADCHANMASK(const std::string &nick, const std::string &chan);

        // │──────────────────────────────────────│ JOIN │──────────────────────────────────────│ //

        // │──────────────────────────────────────│ TOPIC │─────────────────────────────────────│ //

        static std::string TOPIC(const std::string &prefix, const std::string &chan, const std::string &topic);
        static std::string RPL_NOTOPIC(const std::string &nick, const std::string &chan);
        static std::string RPL_TOPIC(const std::string &nick, const std::string &chan, const std::string &topic);
        static std::string ERR_NOSUCHCHANNEL(const std::string &nick, const std::string &chan);
        static std::string ERR_NOTONCHANNEL(const std::string &nick, const std::string &chan);
        static std::string ERR_CHANOPRIVSNEEDED(const std::string &nick, const std::string &chan);

        // │──────────────────────────────────────│ TOPIC │─────────────────────────────────────│ //

        // │──────────────────────────────────────│ PRIVMSG │──────────────────────────────────────│ //

        static std::string ERR_NORECIPIENT(const string& command);
        static std::string ERR_CANNOTSENDTOCHAN(const string& channel);
        static std::string ERR_NOSUCHNICK(const string& nickname);
        static std::string ERR_NOTEXTTOSEND();

        // │──────────────────────────────────────│ PRIVMSG │──────────────────────────────────────│ //
};
