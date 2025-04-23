/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adesille <adesille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:50:01 by adesille          #+#    #+#             */
/*   Updated: 2025/04/23 14:01:14 by adesille         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "ircServ.hpp"

Client::Client(int fd) : fd(fd), authenticated(false) {}

Client::~Client() {
    close(fd);
}

void Client::sendReply(const std::string& message) {
    send(fd, message.c_str(), message.size(), 0);
}

void Client::handleCommand(const std::string& command, const std::vector<std::string>& args) {
    // Command handling implementation goes here
	(void)command;
	(void)args;
}
