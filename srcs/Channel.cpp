/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adesille <adesille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:49:33 by adesille          #+#    #+#             */
/*   Updated: 2025/04/23 14:01:49 by adesille         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "ircServ.hpp"

Channel::Channel(const std::string& name) : name(name), inviteOnly(false), topicRestricted(false), userLimit(-1) {}

void Channel::addClient(Client* client) {
    members[client->fd] = client;
}

void Channel::removeClient(Client* client) {
    members.erase(client->fd);
    operators.erase(client->fd);
}

void Channel::setMode(char mode, bool set, const std::string& arg) {
    switch (mode) {
        case 'i': inviteOnly = set; break;
        case 't': topicRestricted = set; break;
        case 'k': if (set) key = arg; else key = ""; break;
        case 'l': if (set) userLimit = atoi(arg.c_str()); else userLimit = -1; break;
        case 'o': {
            if (set) operators[atoi(arg.c_str())] = members[atoi(arg.c_str())];
            else operators.erase(atoi(arg.c_str()));
            break;
        }
    }

}
