/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : srcs/Server/stop.cpp
** Author     : aheitz
** Created    : 2025-05-16
** Edited     : 2025-05-16
** Description: Enables clean shutdown of server to save data
*/

#include "ircServ.hpp"
#include "log.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

using namespace std;

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

/**
 * Sets 'g_stop' variable to stop the server's main loop when used as callback for signals
 */
extern "C" void stopServer(int signo) {
    LOG_DEBUG("Encountered SIG=" + intToString(signo));
    LOG_WARNING("SERVER IS SHUTING DOWN...");
    g_stop = 1;
};