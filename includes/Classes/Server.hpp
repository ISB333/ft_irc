/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adesille <adesille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:36:39 by adesille          #+#    #+#             */
/*   Updated: 2025/04/23 13:42:56 by adesille         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#pragma  once

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

using namespace std;
// int	ircServ(int port, string password);

class Server {
	private:
		int port;
		std::string password;
		int serverSocket;
		struct sockaddr_in serverAddr;
		vector<struct pollfd> pollfds;
		map<int, Client*> clients;
		map<std::string, Channel*> channels;

		void setupSocket();
		void handleNewConnection();
		void handleClientData(int index);
		void processCommand(Client* client, const std::string& message);
		void removeClient(int fd);
		void broadcastMessage(const std::string& message, Client* sender, Channel* channel = NULL);

	public:
		Server(int port, const std::string& password);
		~Server();
		void run();
};
