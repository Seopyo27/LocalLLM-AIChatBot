#include "ChatHistory.h"

#include <cctype>
#include <iostream>

static std::string toLowerCaseForSearch(const std::string& text)
{
    std::string result;

    for (char ch : text)
    {
        result += static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    }

    return result;
}

ChatHistory::ChatHistory() = default;

ChatHistory::~ChatHistory() = default;

void ChatHistory::addMessage(const std::string& role, const std::string& content)
{
    Message message(role, content);

    // vector stores messages in the same order they are added.
    messages_.push_back(message);
}

void ChatHistory::printHistory() const
{
    if (messages_.empty())
    {
        std::cout << "No conversation history yet.\n\n";
        return;
    }

    std::cout << "Conversation history:\n";

    // This loop visits each saved message in order.
    for (const Message& message : messages_)
    {
        std::cout << message.getRole() << ": " << message.getContent() << '\n';
    }

    std::cout << '\n';
}

void ChatHistory::findMessages(const std::string& keyword) const
{
    if (keyword.empty())
    {
        std::cout << "Please enter a keyword to search for.\n\n";
        return;
    }

    if (messages_.empty())
    {
        std::cout << "No conversation history yet.\n\n";
        return;
    }

    const std::string lowerKeyword = toLowerCaseForSearch(keyword);
    bool found = false;

    std::cout << "Search results for \"" << keyword << "\":\n";

    // The vector is searched from first message to last message.
    for (const Message& message : messages_)
    {
        const std::string lowerContent = toLowerCaseForSearch(message.getContent());

        if (lowerContent.find(lowerKeyword) != std::string::npos)
        {
            std::cout << message.getRole() << ": " << message.getContent() << '\n';
            found = true;
        }
    }

    if (!found)
    {
        std::cout << "No matching messages found.\n";
    }

    std::cout << '\n';
}
