/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : srcs/Server/setupSocket.cpp
** Author     : adesille, aheitz
** Created    : 2025-04-23
** Edited     : 2025-05-12
** Description: Client member functions
*/

#include "ircServ.hpp"

//TODO: The function is dangerous and should check and process send().
void Client::sendReply(const std::string& message) {
    send(fileDescriptor_, (message + CRLF).c_str(), message.size(), 0);
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
