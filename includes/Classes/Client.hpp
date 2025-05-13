/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : includes/Classes/Client.hpp
** Author     : adesille, aheitz
** Created    : 2025-04-22
** Edited     : 2025-05-02
** Description: Every client deserves a structure to track their connection
*/

//TODO: Check the relevance of each member function.
//TODO: The constructor should initiate each field?

#pragma  once

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

#include "ircServ.hpp"

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
        void   appendToInputBuffer(const char input[], const size_t size) { inputBuffer_.append(input, size); };

        /**
         * @brief Retrieves the next line from the buffer
         * 
         * @return std::string The full line
         */
        std::string popInputBuffer(void) {
            size_t position = inputBuffer_.find(CRLF);

            if (position eq std::string::npos)   return "";

            std::string line = inputBuffer_.substr(0, position);
                          inputBuffer_.erase(0,  position + 2);
            return line;
        };

        /**
         * @brief Appends the message (and the CRLF to respect the protocol) to the output
         * 
         * @param message The message to append to the output
         */
        void appendToOutputBuffer(const std::string &message) { outputBuffer_ += message + CRLF; };

        /**
         * @brief Clears output buffer
         * 
         */
        void clearOutputBuffer(void) { outputBuffer_.clear(); };
        void clearInputBuffer(void) { inputBuffer_.clear(); };

        // │────────────────────────────────────────────────────────────────────────────────────│ //

        int          getFileDescriptor(void) const { return                            fileDescriptor_; };
        bool         getAuthentication(void) const { return                             authenticated_; };
        int          getPasswdAttempt(void)  const { return                          password_attempt_; };
        const std::string &getUsername(void)      const { return                                  username_; };
        const std::string &getNickname(void)      const { return                                  nickname_; };
        const std::string &getIP(void)            const { return                                  clientIP_; };
        const std::string getPrefix(void)         const { return nickname_ + "!" + username_ + "@localhost"; };
        const std::string &getInputBuffer(void)   const { return                               inputBuffer_; };
        const std::string &getOutputBuffer(void)  const { return                              outputBuffer_; };

        // │────────────────────────────────────────────────────────────────────────────────────│ //

        void toggleAuthentication(const bool change) {
            if (change not_eq authenticated_)   authenticated_ = not authenticated_;
        };
        bool isFullyRegistered(void) const {
            return authenticated_&& !username_.empty() && !nickname_.empty();
        }

        void setRealname(const std::string realname) {         realname_ = realname; };
        void setUsername(const std::string username) {         username_ = username; };
        void setNickname(const std::string nickname) {      inputBuffer_ = nickname; };
        void incrementPasswdAttempt()           { password_attempt_++;          };
        void sendReply(const std::string& message);
        void handleCommand(const std::string& command, const std::vector<std::string>& args);

    private:
        const int        fileDescriptor_;
        bool              authenticated_;
		int	           password_attempt_;
        std::string            clientIP_;
        std::string            username_;
        std::string            realname_;
        std::string            nickname_;
        std::string         inputBuffer_;
        std::string        outputBuffer_;

        Client(const Client &source);            // Non-instantiable
        Client &operator=(const Client &source); // Non-instantiable
};
