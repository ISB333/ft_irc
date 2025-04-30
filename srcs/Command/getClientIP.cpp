/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getCLientIp.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adesille <adesille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:21:59 by adesille          #+#    #+#             */
/*   Updated: 2025/04/30 14:22:38 by adesille         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "ircServ.hpp"

std::string    getClientIP(int fd)
{
    struct sockaddr_in  addr;
    socklen_t           len = sizeof(addr);

    if (getpeername(fd, (struct sockaddr *)&addr, &len) < 0)
        return ("");

    return (std::string(inet_ntoa(addr.sin_addr)));
}
