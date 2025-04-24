/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adesille <adesille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:47:58 by adesille          #+#    #+#             */
/*   Updated: 2025/04/24 09:46:52 by adesille         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#pragma once

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

using namespace std;

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

class Client;

class Channel {
	public:
		map<int, Client*>	members;
		map<int, Client*>	operators;
		string				name;
		string 				topic;
		string				key;
		bool				inviteOnly;
		bool				topicRestricted;
		int					userLimit;

		Channel(const std::string& name);
		~Channel() { };

		void				addClient(Client* client);
		void				removeClient(Client* client);
		void				setMode(char mode, bool set, const std::string& arg = "");
};
