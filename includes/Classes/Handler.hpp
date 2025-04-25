/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adesille <adesille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 10:00:45 by adesille          #+#    #+#             */
/*   Updated: 2025/04/25 10:35:13 by adesille         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#pragma  once

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

using namespace std;

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

class Server;

class Handler {
	public:
		Handler(Server& server);
		~Handler() {};
		void	dispatchCommand(Client* client, const std::string &message);

		void	handleNick(Client* client, const std::vector<std::string>& args);
		void	handleUser(Client* client, const std::vector<std::string>& args);
		void	handleJoin(Client* client, const std::vector<std::string>& args);
		void	handlePrivmsg(Client* client, const std::vector<std::string>& args);

	private:
		Server&							server;
		typedef void (Handler::*CommandFunction)(Client*, const std::vector<std::string>&);
		map<string, CommandFunction>	commandMap;		
};
