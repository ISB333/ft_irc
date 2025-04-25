
/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : includes/ircServ.hpp
** Author     : adesille, aheitz
** Created    : 2025-04-21
** Edited     : 2025-04-24
** Description: The inclusion head for the project
*/

#pragma once

//FIXME: Remove ALL namespaces from headers.

#include "functions.hpp"
#include "libraries.hpp"
#include "operators.hpp"
#include "macros.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

class Client;
class Channel;
class Server;

#include "Classes/Client.hpp"
#include "Classes/Server.hpp"
#include "Classes/Channel.hpp"
#include "Classes/Handler.hpp"
#include "Classes/Replies.hpp"

#include "Structs/Command.hpp"
