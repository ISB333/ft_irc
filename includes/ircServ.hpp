
/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : includes/ircServ.hpp
** Author     : adesille, aheitz
** Created    : 2025-04-21
** Edited     : 2025-05-13
** Description: The inclusion head for the project
*/

//TODO: Comment functions at the end of the project.

#pragma once

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

#include "functions.hpp"
#include "libraries.hpp"
#include "operators.hpp"
#include "macros.hpp"

//FIXME: Remove ALL namespaces from headers.
using namespace std;

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

class Client;
class Channel;
class Server;
class Handler;

#include "Classes/Client.hpp"
#include "Classes/Server.hpp"
#include "Classes/Channel/Channel.hpp"
#include "Classes/Handler.hpp"
#include "Classes/Replies.hpp"

#include "Structs/Command.hpp"