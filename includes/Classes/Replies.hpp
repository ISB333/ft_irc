/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adesille <adesille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 10:22:55 by adesille          #+#    #+#             */
/*   Updated: 2025/04/30 14:46:19 by adesille         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

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
};
