/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adesille <adesille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 10:22:55 by adesille          #+#    #+#             */
/*   Updated: 2025/04/25 10:27:54 by adesille         ###   ########.fr       */
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
};
