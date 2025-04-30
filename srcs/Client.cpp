/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adesille <adesille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:50:01 by adesille          #+#    #+#             */
/*   Updated: 2025/04/30 12:05:20 by adesille         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "ircServ.hpp"

void Client::sendReply(const std::string& message) {
    send(_fileDescriptor, message.c_str(), message.size(), 0);
}

void Client::handleCommand(const std::string& command, const std::vector<std::string>& args) {
	/*
	{
		// Command handling implementation goes here
	}
	*/
	(void)command;
	(void)args;
}
