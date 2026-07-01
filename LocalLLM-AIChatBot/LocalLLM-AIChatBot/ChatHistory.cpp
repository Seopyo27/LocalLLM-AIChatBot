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

void findMessages(const std::vector<Message>& history, const std::string& keyword)
{
    if (keyword.empty())
    {
        std::cout << "Please enter a keyword to search for.\n\n";
        return;
    }

    if (history.empty())
    {
        std::cout << "No conversation history yet.\n\n";
        return;
    }

    const std::string lowerKeyword = toLowerCaseForSearch(keyword);
    bool found = false;

    std::cout << "Search results for \"" << keyword << "\":\n";

    // The vector is searched from first message to last message.
    for (const Message& message : history)
    {
        const std::string lowerContent = toLowerCaseForSearch(message.content);

        if (lowerContent.find(lowerKeyword) != std::string::npos)
        {
            std::cout << message.role << ": " << message.content << '\n';
            found = true;
        }
    }

    if (!found)
    {
        std::cout << "No matching messages found.\n";
    }

    std::cout << '\n';
}
