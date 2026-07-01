#include "ChatHistory.h"

#include <iostream>

void addMessage(std::vector<Message>& history, const std::string& role, const std::string& content)
{
    Message message;
    message.role = role;
    message.content = content;

    // vector stores messages in the same order they are added.
    history.push_back(message);
}

void printHistory(const std::vector<Message>& history)
{
    if (history.empty())
    {
        std::cout << "No conversation history yet.\n\n";
        return;
    }

    std::cout << "Conversation history:\n";

    // This loop visits each saved message in order.
    for (const Message& message : history)
    {
        std::cout << message.role << ": " << message.content << '\n';
    }

    std::cout << '\n';
}
