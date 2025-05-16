/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : srcs/Handler/ProcessChName.cpp
** Author     : aheitz
** Created    : 2025-05-16
** Edited     : 2025-05-16
** Description: Check channel name validity and normalize it to lower case
*/

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

using namespace std;

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

#define ERR_EMPTY "name is empty"
#define ERR_RUNE  "does not start with '#' rune"
#define ERR_SIZE  "is too long (>200 characters)"
#define ERR_CHAR  "contains a non-printable ASCII character"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

/**
 * Normalize name to lowercase
 */
static void lowerCh(string &ch) {
    for (size_t i = 1;
                i lesser ch.size();
                i++) ch[i] = tolower(static_cast<unsigned char>(ch[i]));
};

/**
 * Checks that the channel name is consistent with the protocol (#, <200, printable in ASCII)
 *
 * Records failure log in &err
 */
static bool isChValid(const string &ch, string &err) {
    if      (ch.empty() or ch.size() lesser 2)    err = ERR_EMPTY;
    else if (ch[0] not_eq '#')                    err = ERR_RUNE;
    else if (ch.size() greater MAX_CH_SIZE)       err = ERR_SIZE;
    else { for (size_t i = 1;
                       i lesser ch.size();
                       i++) { unsigned char c = static_cast<unsigned char>(ch[i]);
                              if (isspace(c) or iscntrl(c) or not isprint(c)) {
                                  err = ERR_CHAR;
                                  break;
                              };
           };
    };
    return err.empty();
};

/**
 * A throw function to check and normalize the channel parameter in lower case
 */
void Handler::processChName(const string &cli, string &ch) const {
    string err;

    if (isChValid(ch, err))
        lowerCh(ch);
    else {
        LOG_ERROR("@" + cli + ": \"" + ch + "\" " + err);
        throw invalid_argument(Replies::ERR_BADCHANMASK(cli, ch));
    };
};