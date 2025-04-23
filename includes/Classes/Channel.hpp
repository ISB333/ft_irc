/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adesille <adesille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:47:58 by adesille          #+#    #+#             */
/*   Updated: 2025/04/23 14:01:43 by adesille         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#pragma once

#include "ircServ.hpp"

class Client;

class Channel {
	public:
		std::string name;
		std::string topic;
		std::map<int, Client*> members;
		std::map<int, Client*> operators;
		std::string key;
		bool inviteOnly;
		bool topicRestricted;
		int userLimit;

		Channel(const std::string& name);
		~Channel() { };
		void addClient(Client* client);
		void removeClient(Client* client);
		void setMode(char mode, bool set, const std::string& arg = "");
};
