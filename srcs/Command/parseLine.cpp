/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : srcs/Command/parseLine.cpp
** Author     : aheitz
** Created    : 2025-04-23
** Edited     : 2025-04-22
** Description: Functionality for parsing and returning a command
*/

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

using namespace std;

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

//TODO: Improve parsing.

/**
 * @brief Parse the line ([‘:’ <prefix> <SPACE> ] <command> <params> [ ‘:’ <trailing> ] <CRLF>) to return a valid command
 * 
 * @param line The line to be processed
 * @return Command The command containing the name and arguments
*/
Command parseLine(std::string line) {
    Command result;
    size_t  nextSpace;

    if (line.size() at_least 2 and line.substr(line.size() - 2) eq CRLF)
        line.erase(line.size() - 2);

    if (line.empty())
        throw invalid_argument("Command line is empty");

    if (line[0] eq ':') {
        nextSpace = line.find(SPACE);
        if (nextSpace eq string::npos)
            throw invalid_argument("Command line has prefix but is empty");
        line.erase(0, nextSpace + 1);
    }

    line.erase(0,  line.find_first_not_of(SPACE));
    if (line.empty())
        throw invalid_argument("Command line is empty");

    nextSpace   = line.find(SPACE);
    if (nextSpace eq string::npos) {
        result.name = line;
        return result;
    }
    result.name = line.substr(0, nextSpace);
                  line.erase(0, nextSpace + 1);

    while (true) {
        size_t start = line.find_first_not_of(SPACE);
        if (start eq string::npos)
            break;
        line.erase(0, start);

        if (line[0] eq ':') {
            result.argv.push_back(line.substr(1));
            break;
        }

        nextSpace = line.find(SPACE);
        if (nextSpace eq string::npos)
			{ result.argv.push_back(line); break; }
        else {
            result.argv.push_back(line.substr(0, nextSpace));
            line.erase(0, nextSpace + 1);
        }
    }
    return result;
};
