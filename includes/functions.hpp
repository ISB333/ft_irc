/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : includes/functions.hpp
** Author     : aheitz
** Created    : 2025-04-24
** Edited     : 2025-04-24
** Description: The complete list of functions used by the project
*/

#pragma once

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

#include "libraries.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

std::string formatReply(const int         code,
                        const std::string &nickname,
                        const std::string &middle,
                        const std::string &text);

std::string intToString(int n);
std::string getClientIP(int fd);
