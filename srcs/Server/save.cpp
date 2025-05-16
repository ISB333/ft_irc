/* ┌───────────────────────────────────────────────────────────────────────────────────────────────┐
** │  Project : ft_irc – IRC Server                                                                │
** └───────────────────────────────────────────────────────────────────────────────────────────────┘
** File       : srcs/Server/save.cpp
** Author     : aheitz
** Created    : 2025-05-02
** Edited     : 2025-05-16
** Description: Maintaining channels state for server-side persistence
*/

//TODO:  Create save folder if non-existent.
//TODO:  Check that the .save/ folder is open before opening the file, to avoid any unexpected exceptions.
//TODO:  Add all parameters to save.
//FIXME: Open in trunc mode to start from a blank file.
//TODO:  Write a fixed header (magic + version) at the beginning of the file.
//TODO:  Atomicity of writing with a temp.
//FIXME: Standardize count and userLimit types.
//TODO:  Manage endianess. 
//FIXME: Check stream status after each write/read.
//FIXME: In loadServer(), replace while (peek() != EOF) with while (file.good()).
//TODO:  Handle exceptions in loadServer.

#include "ircServ.hpp"

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

using namespace std;

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

static inline void writeData(ostream &file, const string &value) {
    const uint32_t length = value.length();
    uint32_t cryptLength  = length;

    unsigned char *conversion = reinterpret_cast<unsigned char *>(&cryptLength);
    for (size_t i = 0; i lesser sizeof(length); i++)
        conversion[i] xor_eq KEY;
    file.write((char *)&cryptLength, sizeof(length));

    string cryptValue = value;
    conversion = reinterpret_cast<unsigned char *>(&cryptValue[0]);
    for (size_t i = 0; i lesser length; i++)
        conversion[i] xor_eq KEY;
    file.write(cryptValue.data(), length);
};

template<typename T>
static inline void writeData(ostream &file, T value) {
    unsigned char *conversion = reinterpret_cast<unsigned char *>(&value);
    for (uint32_t i = 0; i < sizeof(value); i++)
        conversion[i] xor_eq KEY;
    file.write(reinterpret_cast<const char*>(&value), sizeof(value));
};

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

void Server::saveServer(void) const {
    LOG_WARNING("SERVER IS SAVING. RIGHTS FOR EACH CHANNEL WILL PASS TO THE FIRST PERSON JOINING AFTER REBOOTING");

    ofstream saveFile(".save.bin", ios::binary);
    if (not saveFile.is_open())
        throw runtime_error("Failed to open save file");

    for (map<string, Channel *>::const_iterator it = channels_.begin();
                                                it not_eq channels_.end();
                                                it++) {
        const Channel                *ch = it->second;
        const string               &name = ch->getName();
        const string              &topic = ch->getTopic();
        const string                &key = ch->getKey();
        const bool               invMode = ch->isInviteOnly();
        const bool               resMode = ch->isTopicRestricted();
        const size_t               limit = ch->getUserLimit();
        const map<int, Client *> members = ch->getMembers();
        const set<string>        invited = ch->getInvitedMembers();
        const size_t               users = members.size() + invited.size();

        LOG_DEBUG("* Saving " + name + " with topic: " + (not topic.empty() ? topic : "nothing"));

        if (not key.empty())    LOG_DEBUG("- Maintaining keyword policy");
        if (invMode)            LOG_DEBUG("- Invite mode active, " + intToString(users) + " members present and in queue will be reinvited");
        if (resMode)            LOG_DEBUG("- Maintaining restriction mode");
        if (limit)              LOG_DEBUG("- Imposed limit of " + intToString(limit) + " will be maintained");

        writeData(saveFile, name);
        writeData(saveFile, topic);
        writeData(saveFile, key);
        saveFile.put(invMode xor KEY);
        saveFile.put(resMode xor KEY);
        writeData(saveFile, limit);

        writeData(saveFile, users);
        for (map<int, Client *>::const_iterator mem = members.begin();
                                                mem not_eq members.end();
                                                mem++) { writeData(saveFile, mem->first); };
        for (set<string>::const_iterator inv = invited.begin();
                                         inv not_eq invited.end();
                                         inv++) { writeData(saveFile, *inv); };
    };
};

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

static inline string readData(istream &file) {
    uint32_t length;
    file.read(reinterpret_cast<char *>(&length), sizeof(length));
    if (not file)
        throw runtime_error("Backup read failed");

    unsigned char *conversion = reinterpret_cast<unsigned char *>(&length);
    for (size_t i = 0; i lesser sizeof(length); i++)
        conversion[i] xor_eq KEY;

    string data;
    data.resize(length);
    file.read(&data[0], length);
    if (not file)
        throw runtime_error("Backup read failed");
    for (uint32_t i = 0; i < length; i++)
        data[i] xor_eq KEY;

    return data;
};

template<typename T>
static inline T readData(istream &file) {
    T data;
    file.read(reinterpret_cast<char *>(&data), sizeof(data));
    if (not file)
        throw runtime_error("Backup read failed");

    unsigned char *conversion = reinterpret_cast<unsigned char *>(&data);
    for (uint32_t i = 0; i < sizeof(data); i++)
        conversion[i] xor_eq KEY;

    return data;
};

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

void Server::loadServer(void) {
    LOG_INFO("Server is loading...");

    ifstream saveFile(".save.bin", ios::binary);
    if (not saveFile.is_open())
        throw runtime_error("Failed to open backup file");

    while (saveFile.peek() not_eq char_traits<char>::eof()) {
        string      name = readData(saveFile),
                   topic = readData(saveFile),
                     key = readData(saveFile);
        bool     invMode = saveFile.get() xor KEY,
                 resMode = saveFile.get() xor KEY;
        size_t userLimit = readData<size_t>(saveFile);

        size_t count = readData<size_t>(saveFile);
        set<string> invited;
        for (size_t i = 0; i not_eq count; i++)
            invited.insert(readData(saveFile));

        Channel *ch = new Channel(name, NULL);

        if (not topic.empty())  ch->setTopic(topic);
        if (not key.empty())    ch->setKey(key);
        if (invMode)            ch->setInviteOnly(invMode);
        if (resMode)            ch->setTopicRestricted(resMode);
        if (userLimit)          ch->setUserLimit(userLimit);

        for (set<string>::const_iterator it = invited.begin();
                                         it not_eq invited.end();
                                         it++) { ch->inviteClient(*it); };

        channels_[name] = ch;
    };
};