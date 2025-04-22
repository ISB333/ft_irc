/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : srcs/main.cpp
** Author     : adesille, aheitz
** Created    : 2025-04-21
** Edited     : 2025-04-22
** Description: TO BE DEFINED
*/
//TODO: Define header description.

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

using namespace std;

#define USAGE "./ircserv <port> <password>"
#define RANGE "(1-65535)"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

static void checkArgc(const int  argc);
static int  parsePort(const char asciiPort[]);

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

//TODO: Complete and document the hand.
/**
 * @brief 
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char *argv[]) {
    try {
        checkArgc(argc);
        int    port     = parsePort(argv[1]);   (void)port; //TODO: Remove this instruction.
        string password = argv[2];
        if (password.empty())   throw invalid_argument("A password should be defined");

        exit(EXIT_SUCCESS);
    } catch (const exception &error) {
        cerr << "SERVER SHUTDOWN - Error encountered\n" << error.what() << endl;
        exit(EXIT_FAILURE);
    }
};

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

/**
 * @brief A simple function to deal with invalid argument numbers
 * 
 * @param argc The number of arguments transmitted by the program
 */
static void checkArgc(const int argc) {
    if (argc lesser  3)     throw invalid_argument("Too few arguments: "    USAGE);
    if (argc greater 3)     throw invalid_argument("Too many arguments: "   USAGE);
};

/**
 * @brief A simple function to ensure that the port transmitted is a valid number
 * 
 * @param asciiPort The port in alphabetical format
 * @return int The port in digital format
 */
static int parsePort(const char asciiPort[]) {
    errno            = 0;
    char *end        = NULL;
    long digitalPort = strtol(asciiPort, &end, 10);

    if (end eq asciiPort)                               throw invalid_argument("The port contains no digits");
    if (*end)                                           throw invalid_argument(string("Invalid character in port: ") + *end);
    if (errno eq ERANGE or digitalPort greater 65535)   throw invalid_argument("Port superior to the valid range: " RANGE);
    if (digitalPort lesser 1)                           throw invalid_argument("Port below valid range: "           RANGE);

    return static_cast<int>(digitalPort);
};