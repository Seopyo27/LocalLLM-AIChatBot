#pragma once

#include <string>
#include <vector>

#include "Message.h"

class ChatHistory
{
public:
    ChatHistory();
    ~ChatHistory();

    void addMessage(const std::string& role, const std::string& content);
    void printHistory() const;
    void findMessages(const std::string& keyword) const;

private:
    std::vector<Message> messages_;
};
