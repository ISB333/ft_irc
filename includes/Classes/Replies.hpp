/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheitz <aheitz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 10:22:55 by adesille          #+#    #+#             */
/*   Updated: 2025/05/15 09:53:30 by aheitz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma  once

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

using namespace std;

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

class Replies {
    public:
        static std::string ERR_UNKNOWNCOMMAND(const std::string& nickname, const std::string& command) {
            return ":server 421 " + nickname + " " + command + " :Unknown command\r\n";
        }

        static std::string ERR_ERRONEUSNICKNAME(const std::string& nickname) {
            return ":server 432 " + nickname + " :Erroneous nickname\r\n";
        }

        static std::string ERR_NICKNAMEINUSE(const std::string& nickname) {
            return ":server 433 " + nickname + " :Nickname is already in use\r\n";
        }

        static std::string ERR_ERRONEUSUSERNAME(const std::string& username) {
            return ":server 468 " + username + " :Erroneous username\r\n";
        }

        static std::string ERR_ERRONEUSREALNAME(const std::string& realname) {
            return ":server 468 " + realname + " :Erroneous realname\r\n";
        }

        static std::string ERR_PASSWDMISMATCH(const std::string& passwd, const int attempts) {
            return ":server 464 " + passwd + " :Password incorrect, " + intToString(3 - attempts) + " attempts left\r\n";
        }

        static std::string ERR_YOUWILLBEBANNED() {
            return ":server 466 WARNING :You will be Banned\r\n";
        }

        static std::string ERR_YOUREBANNEDCREEP() {
            return ":server 465 GOULAG CHIEF :You're now Banned for Life and it's too late to cry\r\n";
        }

        static std::string ERR_UNKNOWNERROR(const std::string& target, const std::string& command, const std::string& message) {
            return ":server 400 " + target + " " + command + " :" + message + "\r\n";
          }

        static std::string ERR_ALREADYBANNED() {
            return ":server 666 GTFO :You've already been Banned! Adios Boloss!\r\n";
          }

        static std::string ERR_NOTREGISTERED(const std::string& command) {
            return ":server 451 " + command + " :You have not fully registered (PASS, NICK and USER necessary)\r\n";
        }

        static std::string ERR_NEEDMOREPARAMETERS(const std::string& command) {
            return ":server 461 * " + command + " :Not enough parameters\r\n";
        }

        static std::string ERR_ALREADYREGISTERED() {
            return ":server 462 * :You may not reregister\r\n";
        }

        // │──────────────────────────────────────│ JOIN │──────────────────────────────────────│ //

        static std::string JOIN(const std::string &prefix, const std::string &chan) {
            return ':' + prefix + " JOIN " + chan;
        };

        static std::string RPL_NAMREPLY(const std::string &nick, const Channel *chan) {
            std::ostringstream stream;
            stream << ':' << SERVER_NAME << " 353 " << nick << " = " << chan->getName() << " :";

            const std::map<int, Client *>                 &members = chan->getMembers();
                  std::map<int, Client *>::const_iterator  m       = members.begin();
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

        static std::string RPL_ENDOFNAMES(const std::string &nick, const std::string &chan) {
            return ':' + string(SERVER_NAME) + " 366 " + nick + ' ' + chan + " :End of NAMES list";
        };
        
        static std::string ERR_USERONCHANNEL(const std::string &nick, const std::string &chan) {
            return ':' + string(SERVER_NAME) + " 443 " + nick + ' ' + chan + " :is already on channel";
        };

        static std::string ERR_CHANNELISFULL(const std::string &nick, const std::string &chan) {
            return ':' + string(SERVER_NAME) + " 471 " + nick + ' ' + chan + " :Cannot join channel (+l)";
        };

        static std::string ERR_INVITEONLYCHAN(const std::string &nick, const std::string &chan) {
            return ':' + string(SERVER_NAME) + " 473 " + nick + ' ' + chan + " :Cannot join channel (+i)";
        };
        
        static std::string ERR_BADCHANNELKEY(const std::string &nick, const std::string &chan) {
            return ':' + string(SERVER_NAME) + " 475 " + nick + ' ' + chan + " :Cannot join channel (+k)";
        };

        static std::string ERR_BADCHANMASK(const std::string &nick, const std::string &chan) {
            return ':' + string(SERVER_NAME) + " 476 " + nick + ' ' + chan + " :Bad Channel Mask";
        };

        // │──────────────────────────────────────│ JOIN │──────────────────────────────────────│ //

        // │──────────────────────────────────────│ TOPIC │─────────────────────────────────────│ //

        static std::string TOPIC(const std::string &prefix, const std::string &chan, const std::string &topic) {
            return ':' + prefix + " TOPIC " + chan + " :" + topic;
        };
        
        static std::string RPL_NOTOPIC(const std::string &nick, const std::string &chan) {
            return ':' + string(SERVER_NAME) + " 331 " + nick + ' ' + chan + " :No topic is set";
        };
        
        static std::string RPL_TOPIC(const std::string &nick, const std::string &chan, const std::string &topic) {
            return ':' + string(SERVER_NAME) + " 332 " + nick + ' ' + chan + " :" + topic;
        };

        static std::string ERR_NOSUCHCHANNEL(const std::string &nick, const std::string &chan) {
            return ':' + string(SERVER_NAME) + " 403 " + nick + ' ' + chan + " :No such channel";
        };

        static std::string ERR_NOTONCHANNEL(const std::string &nick, const std::string &chan) {
            return ':' + string(SERVER_NAME) + " 442 " + nick + ' ' + chan + " :You're not on that channel";
        };

        static std::string ERR_CHANOPRIVSNEEDED(const std::string &nick, const std::string &chan) {
            return ':' + string(SERVER_NAME) + " 482 " + nick + ' ' + chan + " :You're not channel operator";
        };

        // │──────────────────────────────────────│ TOPIC │─────────────────────────────────────│ //
};