/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : includes/Classes/Client.hpp
** Author     : adesille, aheitz
** Created    : 2025-04-22
** Edited     : 2025-05-10
** Description: Every client deserves a structure to track their connection
*/

//TODO: Check the relevance of each member function.
//TODO: The constructor should initiate each field?

#pragma  once

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

using namespace std;

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

/**
 * @brief The class containing all the information and functions for connected clients
 * 
 */
class Client {
    public:
        /**
         * @brief Construct a new Client object
         * 
         * @param fd Client ID
         */
        explicit Client(const int fd) : fileDescriptor_(fd), authenticated_(false), password_attempt_(0), clientIP_(getClientIP(fd)) {};
        ~Client() { close(fileDescriptor_); }

        // │────────────────────────────────────────────────────────────────────────────────────│ //

        /**
         * @brief Adds received data to input buffer
         * 
         * @param input Data to be processed 
         * @param size The size of data
         */
        void   appendToInputBuffer(const char input[], const size_t size) {
            lastActivity_ = time(NULL);
            inputBuffer_.append(input, size);
        };

        /**
         * @brief Retrieves the next line from the buffer
         * 
         * @return string The full line
         */
        string popInputBuffer(void) {
            size_t position = inputBuffer_.find(CRLF);

            if (position eq string::npos)   return "";

            string line = inputBuffer_.substr(0, position);
                          inputBuffer_.erase(0,  position + 2);
            return line;
        };

        /**
         * @brief Appends the message (and the CRLF to respect the protocol) to the output
         * 
         * @param message The message to append to the output
         */
        void appendToOutputBuffer(const string &message) {
            lastActivity_ = time(NULL);
            outputBuffer_ += message + CRLF;
        };

        /**
         * @brief Clears output buffer
         * 
         */
        void clearOutputBuffer(void) { outputBuffer_.clear(); };

        // │────────────────────────────────────────────────────────────────────────────────────│ //

        int          getFileDescriptor(void) const { return                            fileDescriptor_; };
        int          getLastActivity(void)   const { return                              lastActivity_; };
        bool         getAuthentication(void) const { return                             authenticated_; };
        int          getPasswdAttempt(void)  const { return                          password_attempt_; };
        const string &getUsername(void)      const { return                                  username_; };
        const string &getNickname(void)      const { return                                  nickname_; };
        const string &getIP(void)            const { return                                  clientIP_; };
        const string getPrefix(void)         const { return nickname_ + "!" + username_ + "@localhost"; };
        const string &getInputBuffer(void)   const { return                               inputBuffer_; };
        const string &getOutputBuffer(void)  const { return                              outputBuffer_; };

        // │────────────────────────────────────────────────────────────────────────────────────│ //

        void toggleAuthentication(const bool change) {
            if (change not_eq authenticated_)   authenticated_ = not authenticated_;
        };

        void setUsername(const string username) {         username_ = username; };
        void setNickname(const string nickname) {      inputBuffer_ = nickname; };
        void incrementPasswdAttempt()           { password_attempt_++;          };
        void sendReply(const std::string& message);
        void handleCommand(const std::string& command, const std::vector<std::string>& args);

        void updateActivity(void) { lastActivity_ = time(NULL); };

    private:
        const int   fileDescriptor_;
        bool         authenticated_;
		int	      password_attempt_;
        string            clientIP_;
        string            username_;
        string            nickname_;
        string         inputBuffer_;
        string        outputBuffer_;
        time_t        lastActivity_;

        Client(const Client &source);            // Non-instantiable
        Client &operator=(const Client &source); // Non-instantiable
};
