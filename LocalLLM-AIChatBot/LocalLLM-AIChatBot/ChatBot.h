#pragma once

#include <map>
#include <memory>
#include <string>

#include "ChatHistory.h"

class ChatBot
{
public:
    ChatBot();
    ~ChatBot();

    void run();

private:
    std::unique_ptr<ChatHistory> history_;
    std::map<std::string, std::string> keywordResponses_;

    void printWelcome() const;
    void printHelp() const;
    std::string trimLeadingSpaces(const std::string& text) const;
    bool startsWithCommand(const std::string& input, const std::string& command) const;
    std::string getCommandArgument(const std::string& input, const std::string& command) const;
    bool parseTeachCommand(const std::string& input, std::string& keyword, std::string& response) const;
};
