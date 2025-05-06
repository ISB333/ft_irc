/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : includes/libraries.hpp
** Author     : aheitz
** Created    : 2025-04-22
** Edited     : 2025-05-06
** Description: List of macros used throughout the project
*/

#pragma once

#define SERVER_NAME "ft_irc"

#define CRLF        "\r\n"
#define SPACE       " "

#define RPL_NOTOPIC          331
#define RPL_TOPIC            332
#define ERR_NOSUCHCHANNEL    403
#define ERR_NOTONCHANNEL     442
#define ERR_NEEDMOREPARAMS   461
#define ERR_UNKNOWNMODE      472
#define ERR_CHANOPRIVSNEEDED 482

#define KEY                  0x5A