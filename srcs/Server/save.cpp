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

/**
 * @brief Writes and crypts the length of a string and its value to a file
 *
 * @param file The file to write to
 * @param value The data to be saved
 */
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

/**
 * @brief Writes and crypts a data value to a file
 *
 * @tparam T Can take any type (except string, which has its own overload)
 * @param file The file to write to
 * @param value The data to be saved
 */
template<typename T>
static inline void writeData(ostream &file, T value) {
    unsigned char *conversion = reinterpret_cast<unsigned char *>(&value);
    for (uint32_t i = 0; i < sizeof(value); i++)
        conversion[i] xor_eq KEY;
    file.write(reinterpret_cast<const char*>(&value), sizeof(value));
};

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

/**
 * @brief Saves all channels in a binary file
 * 
 */
void Server::saveServer(void) const {
    ofstream saveFile(".save/channels.bin", ios::binary);
    if (not saveFile.is_open())
        throw runtime_error("Failed to open backup file");

    for (map<string, Channel *>::const_iterator it = channels_.begin(); it not_eq channels_.end(); it++) {
        const Channel *channel = it->second;

        writeData(saveFile, channel->getName());
        writeData(saveFile, channel->getTopic());
        writeData(saveFile, channel->getKey());
        saveFile.put(channel->isInviteOnly()      xor KEY);
        saveFile.put(channel->isTopicRestricted() xor KEY);
        writeData(saveFile, channel->getUserLimit());

        writeData(saveFile, channel->getMembers().size());
        for (map<int, Client *>::const_iterator mem = channel->getMembers().begin(); mem not_eq channel->getMembers().end(); mem++)
            writeData(saveFile, mem->first);

        writeData(saveFile, channel->getOperators().size());
        for (map<int, Client *>::const_iterator op = channel->getOperators().begin(); op not_eq channel->getOperators().end(); op++)
            writeData(saveFile, op->first);

        writeData(saveFile, channel->getInvitedMembers().size());
        for (set<int>::const_iterator inv = channel->getInvitedMembers().begin(); inv not_eq channel->getInvitedMembers().end(); inv++)
            writeData(saveFile, *inv);
    };
};

// │────────────────────────────────────────────────────────────────────────────────────────────│ //

/**
 * @brief Reads and decrypts a saved dataset
 *
 * @param file The backup file
 * @return string Data in string format
 */
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

/**
 * @brief Reads and decrypts a saved dataset
 *
 * @tparam T Can take any type (except string, which has its own overload)
 * @param file The backup file
 * @return T Decrypted data
 */
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

/**
 * @brief Function for loading all server channels
 * 
 */
void Server::loadServer(void) {
    ifstream saveFile(".save/channels.bin", ios::binary);
    if (not saveFile.is_open())
        throw runtime_error("Failed to open backup file");

    while (saveFile.peek() not_eq char_traits<char>::eof()) {
        string        name       = readData(saveFile), topic = readData(saveFile), key = readData(saveFile);
        bool          inviteMode = saveFile.get() xor KEY, topicRestriction = saveFile.get() xor KEY;
        size_t        userLimit  = readData<size_t>(saveFile);

        size_t count = readData<size_t>(saveFile);
        set<int> members;
        for (size_t i = 0; i not_eq count; i++)
            members.insert(readData<int>(saveFile));

        count = readData<size_t>(saveFile);
        set<int> operators;
        for (size_t i = 0; i not_eq count; i++)
            operators.insert(readData<int>(saveFile));

        count = readData<size_t>(saveFile);
        set<int> invited;
        for (size_t i = 0; i not_eq count; i++)
            invited.insert(readData<int>(saveFile));

        Channel *channel = new Channel(name, NULL);
        channel->setTopic(topic);
        channel->setKey(key);
        channel->setInviteOnly(inviteMode);
        channel->setTopicRestricted(topicRestriction);
        channel->setUserLimit(userLimit);

        // for (set<int>::const_iterator it = members.begin(); it not_eq members.end(); it++) {
        //     //TODO: Deal with client backup later. For now, heresy.
        //     // channel->addClient(new Client(*it));
        // };

        for (set<int>::const_iterator it = operators.begin(); it not_eq operators.end(); it++)
            channel->promoteOperator(*it);

        for (set<int>::const_iterator it = invited.begin(); it not_eq invited.end(); it++)
            channel->inviteClient(*it);

        channels_[name] = channel;
    };
};