/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : includes/log.hpp
** Author     : aheitz
** Created    : 2025-05-14
** Edited     : 2025-05-14
** Description: TODO:
*/

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

#pragma once

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

#define RESET  "\033[0m"
#define RED    "\033[1;31m"
#define GREEN  "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE   "\033[1;34m"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

namespace Logger {
    enum Level {DEBUG, INFO, WARNING, ERROR};

    #ifndef LOG_LEVEL
    #define LOG_LEVEL INFO
    #endif

    inline const char *getId(const Level lvl) {
        switch (lvl) {
            case DEBUG:   return "DEBUG";
            case INFO:    return "INFO";
            case WARNING: return "WARNING";
            case ERROR:   return "ERROR";
            default:      return "?";
        };
    };

        inline const char *getColor(const Level lvl) {
        switch (lvl) {
            case DEBUG:   return   BLUE;
            case INFO:    return  GREEN;
            case WARNING: return YELLOW;
            case ERROR:   return    RED;
            default:      return  RESET;
        };
    };

    inline void log(const Level lvl, const std::string &msg) {
        if (lvl at_least LOG_LEVEL)
            std::cerr << getColor(lvl)
                      << '[' << getId(lvl) << "] "
                      << msg << RESET << std::endl;
    };
};

#define LOG_DEBUG(msg)   Logger::log(Logger::DEBUG,   msg)
#define LOG_INFO(msg)    Logger::log(Logger::INFO,    msg)
#define LOG_WARNING(msg) Logger::log(Logger::WARNING, msg)
#define LOG_ERROR(msg)   Logger::log(Logger::ERROR,   msg)