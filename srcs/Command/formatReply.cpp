/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : srcs/Command/formatReply.cpp
** Author     : aheitz
** Created    : 2025-04-24
** Edited     : 2025-04-24
** Description: Function definition for efficiently concatenating a response to the server
*/

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

using namespace std;

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

/**
 * @brief Intelligently formats the set of fields for the client's output buffer
 * 
 * @param code the command's execution code
 * @param nickname the target client's name
 * @param middle any intermediate field for the response (e.g. a channel)
 * @param text the command's textual argument
 * @return string the command's total concatenation for the client's output
 */
string formatReply(const int code, const string &nickname, const string &middle, const string &text) {
    ostringstream oss;

    oss << ":"  << SERVER_NAME
        << " "  << setw(3) << setfill('0') << code << setfill(' ')
        << " "  << nickname
        << " "  << middle
        << " :" << text;
    return oss.str();
};