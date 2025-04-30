/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adesille <adesille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 10:22:55 by adesille          #+#    #+#             */
/*   Updated: 2025/04/30 11:08:22 by adesille         ###   ########.fr       */
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

		static std::string ERR_PASSWDMISMATCH(const std::string& passwd) {
			return ":server 464 " + passwd + " :Password incorrect\r\n";
		}

		static std::string ERR_UNKNOWNERROR(const std::string& target, const std::string& command, const std::string& message) {
    	    return ":server 400 " + target + " " + command + " :" + message + "\r\n";
  		}
};
