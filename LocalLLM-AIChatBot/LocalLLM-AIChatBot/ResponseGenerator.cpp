#include "ResponseGenerator.h"

#include <cctype>
#include <iostream>

static std::string toLowerCase(const std::string& text)
{
    std::string result;

    // This loop copies each character while practicing string traversal.
    for (char ch : text)
    {
        result += static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    }

    return result;
}

static std::string normalizeKeyword(const std::string& keyword)
{
    return toLowerCase(keyword);
}

std::map<std::string, std::string> createDefaultResponses()
{
    std::map<std::string, std::string> keywordResponses;

    // map connects each keyword with the response that should be used for it.
    keywordResponses["bye"] = "Goodbye! Type /exit when you are ready to quit.";
    keywordResponses["c++"] = "C++ is a great language for learning structure and memory basics.";
    keywordResponses["cpp"] = "C++ is a great language for learning structure and memory basics.";
    keywordResponses["hello"] = "Hello! I am a small rule-based learning chatbot.";
    keywordResponses["help"] = "You can type /help to see the command list.";
    keywordResponses["hi"] = "Hello! I am a small rule-based learning chatbot.";
    keywordResponses["name"] = "My name is Local Learning Bot.";

    return keywordResponses;
}

std::string generateResponse(const std::string& userInput, const std::map<std::string, std::string>& keywordResponses)
{
    const std::string lowerInput = toLowerCase(userInput);

    // Searching the map shows how key-value data can drive simple behavior.
    for (const std::pair<const std::string, std::string>& entry : keywordResponses)
    {
        if (lowerInput.find(entry.first) != std::string::npos)
        {
            return entry.second;
        }
    }

    return "I do not understand deeply yet, but I saved your message.";
}

void printKeywordResponses(const std::map<std::string, std::string>& keywordResponses)
{
    if (keywordResponses.empty())
    {
        std::cout << "No keyword responses registered.\n\n";
        return;
    }

    std::cout << "Keyword responses:\n";

    // map keeps keys in sorted order, so the output is predictable.
    for (const std::pair<const std::string, std::string>& entry : keywordResponses)
    {
        std::cout << entry.first << " -> " << entry.second << '\n';
    }

    std::cout << '\n';
}

void teachResponse(std::map<std::string, std::string>& keywordResponses, const std::string& keyword, const std::string& response)
{
    const std::string normalizedKeyword = normalizeKeyword(keyword);

    // Using [] adds a new key or updates the existing value.
    keywordResponses[normalizedKeyword] = response;
}

bool unteachResponse(std::map<std::string, std::string>& keywordResponses, const std::string& keyword)
{
    const std::string normalizedKeyword = normalizeKeyword(keyword);
    std::map<std::string, std::string>::iterator found = keywordResponses.find(normalizedKeyword);

    if (found == keywordResponses.end())
    {
        return false;
    }

    keywordResponses.erase(found);
    return true;
}
