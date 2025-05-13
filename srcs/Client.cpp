/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : srcs/Client/C.cpp
** Author     : adesille, aheitz
** Created    : 2025-04-23
** Edited     : 2025-05-13
** Description: Client member functions
*/

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

using namespace std;

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

string setIp(const int fd) {
    struct sockaddr_in addr;
    socklen_t          len = sizeof(addr);

    return getpeername(fd, reinterpret_cast<struct sockaddr*>(&addr), &len) lesser 0 ? "" : string(inet_ntoa(addr.sin_addr));
};

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

Client::Client(const int fd) : fd_(fd), auth_(false), attempt_(0), ip_(setIp(fd)),
                               username_(""), nickname_(""),
                               input_(""),    output_(""),
                               activity_(time(NULL)) {};

Client::~Client(void) { close(fd_); }

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

void   Client::appendInput(const char input[], const size_t size) {
    activity_ = time(NULL);
    input_.append(input, size);
};

string Client::popInput(void) {
    const size_t position = input_.find(CRLF);
    if (position eq string::npos)
        return "";

    const string line = input_.substr(0, position);
        input_.erase(0, position + 2);
    return line;
};

void Client::appendOutput(const string &message) {
    activity_  = time(NULL);
    output_   += message + CRLF;
};

void Client::consumeOutput(const size_t n) { output_.erase(0, n); };

// │────────────────────────────────────────────────────────────────────────────────────│ //

int           Client::getFd(void)           const { return                                        fd_; };
int           Client::getActivity(void)     const { return                                  activity_; };
bool          Client::isAuthenticated(void) const { return                                      auth_; };
bool          Client::isPassAuth(void)const       { return                                  authPass_; };
int           Client::getAttempt(void)      const { return                                   attempt_; };
const string &Client::getUsername(void)     const { return                                  username_; };
const string &Client::getNickname(void)     const { return                                  nickname_; };
const string &Client::getIp(void)           const { return                                        ip_; };
const string  Client::getPrefix(void)       const { return nickname_ + "!" + username_ + "@localhost"; };
const string &Client::getInput(void)        const { return                                     input_; };
const string &Client::getOutput(void)       const { return                                    output_; };

// │────────────────────────────────────────────────────────────────────────────────────│ //

void Client::authenticate(const bool change)     { auth_     = change;   };
void Client::authenticatePass(const bool change) { authPass_ = change;   };
void Client::setUsername(const string &username) { username_ = username; };
void Client::setNickname(const string &nickname) { nickname_ = nickname; };
void Client::setRealname(const string realname)  { realname_ = realname; };
void Client::incrementAttempt(void)              { ++attempt_;           };

void Client::updateActivity(void)                { activity_  = time(NULL); };

//TODO: The function is dangerous and should check and process send().
void Client::sendReply(const std::string& message) {
    std::string fullMessage = message + CRLF;
    ssize_t bytesSent = send(fd_, fullMessage.c_str(), fullMessage.size(), 0);
    
    if (bytesSent < 0) {
        // Gestion d'erreur
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            // Socket non prêt pour écriture, stocker le message et réessayer plus tard
            appendOutput(message);  // Utilise votre système existant
            return;
            // return 0;
        }
        return;
        // return -1;  // Autre erreur
    } else if (static_cast<size_t>(bytesSent) < fullMessage.size()) {
        // Envoi partiel, stocker le reste pour plus tard
        appendOutput(fullMessage.substr(bytesSent));
        return;
        // return bytesSent;
    }
    
    return;
    // return bytesSent;
}
// void Client::sendReply(const std::string& message) {
//     send(fd_, (message + CRLF).c_str(), message.size(), 0);
// 	// appendOutput(message);
// }

bool Client::isFullyRegistered(void) const {
	return auth_&& !username_.empty() && !nickname_.empty();
} 
