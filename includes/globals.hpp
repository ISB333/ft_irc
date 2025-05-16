/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : includes/globals.hpp
** Author     : aheitz
** Created    : 2025-05-16
** Edited     : 2025-05-16
** Description: Non-exhaustive list of global variables within the project
*/

#pragma once

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

/**
 * Tells the server if it should stop
 */
extern volatile sig_atomic_t g_stop;