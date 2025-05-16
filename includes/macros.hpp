/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : includes/libraries.hpp
** Author     : aheitz
** Created    : 2025-04-22
** Edited     : 2025-05-16
** Description: List of macros used throughout the project
*/

#pragma once

#define SERVER_NAME "ft_irc"

#define CRLF        "\r\n"
#define SPACE       " "

#define KEY                  0x5A

#define POLL_TIMEOUT         1000
#define INACTIVITY_SECONDS   300

#define IRC_LIMIT            512
#define MAX_CH_SIZE          200