/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : ~/Desktop/IRC/ft_irc-V2/srcs/Command/getClientIP.cpp
** Author     : adesille, aheitz
** Created    : 2025-05-06
** Edited     : 2025-05-06
** Description: 
*/

#include "ircServ.hpp"

string    getClientIP(int fd)
{
    struct sockaddr_in  addr;
    socklen_t           len = sizeof(addr);

    if (getpeername(fd, (struct sockaddr *)&addr, &len) < 0)
        return ("");

    return (string(inet_ntoa(addr.sin_addr)));
}
