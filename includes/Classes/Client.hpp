/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : includes/Classes/client.hpp
** Author     : aheitz
** Created    : 2025-04-22
** Edited     : 2025-04-22
** Description: Every client deserves a structure to track their connection
*/

//TODO: Check the relevance of each member function.

#pragma  once

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

using namespace std;

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

class Channel;

/**
 * @brief The class containing all the information and functions for connected clients
 * 
 */
class Client {
    public:
        map<std::string, Channel*>  channels;
        string                      nickname;
        string                      username;
        string                      realname;
        string                      awayMessage;
        string                      buffer;
        int                         fd;
        bool                        authenticated;

        Client(int fd);
        ~Client();

        void                        sendReply(const std::string& message);
        void                        handleCommand(const std::string& command, const std::vector<std::string>& args);
};

// class Client {
//     public:
//         /**
//          * @brief Construct a new Client object
//          * 
//          * @param fd Client ID
//          */
//         explicit Client(const int fd) : _fileDescriptor(fd), _authenticated(false) {};

//         // │────────────────────────────────────────────────────────────────────────────────────│ //

//         /**
//          * @brief Adds received data to input buffer
//          * 
//          * @param input Data to be processed 
//          * @param size The size of data
//          */
//         void   appendToInputBuffer(const char input[], const size_t size) { _inputBuffer.append(input, size); };

//         /**
//          * @brief Retrieves the next line from the buffer
//          * 
//          * @return string The full line
//          */
//         string popInputBuffer(void) {
//             size_t position = _inputBuffer.find(CRLF);

//             if (position eq string::npos)   return "";

//             string line = _inputBuffer.substr(0, position);
//                           _inputBuffer.erase(0,  position + 2);
//             return line;
//         };

//         /**
//          * @brief Appends the message (and the CRLF to respect the protocol) to the output
//          * 
//          * @param message The message to append to the output
//          */
//         void appendToOutputBuffer(const string &message) { _outputBuffer += message + CRLF; };

//         /**
//          * @brief Clears output buffer
//          * 
//          */
//         void clearOutputBuffer(void) { _outputBuffer.clear(); };

//         // │────────────────────────────────────────────────────────────────────────────────────│ //

//         int          getFileDescriptor(void) const { return _fileDescriptor; };
//         bool         getAuthentication(void) const { return _authenticated;  };
//         const string &getUsername(void)      const { return _username;       };
//         const string &getNickname(void)      const { return _nickname;       };
//         const string &getInputBuffer(void)   const { return _inputBuffer;    };
//         const string &getOutputBuffer(void)  const { return _outputBuffer;   };

//         // │────────────────────────────────────────────────────────────────────────────────────│ //

//         void toggleAuthentication(const bool change) {
//             if (change not_eq _authenticated)   _authenticated = not _authenticated;
//         };

//         void setUsername(const string username) { _username = username; };
//         void setNickname(const string nickname) { _nickname = nickname; };
//     private:
//         const int _fileDescriptor;
//         bool      _authenticated;
//         string    _username;
//         string    _nickname;
//         string    _inputBuffer;
//         string    _outputBuffer;

//         Client(const Client &source);            // Non-instantiable
//         Client &operator=(const Client &source); // Non-instantiable
// };
