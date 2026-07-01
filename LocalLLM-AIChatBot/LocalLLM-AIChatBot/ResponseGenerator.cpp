#include "ResponseGenerator.h"

#include <cctype>

std::string toLowerCase(const std::string& text)
{
    std::string result;

    // This loop copies each character while practicing string traversal.
    for (char ch : text)
    {
        result += static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    }

    return result;
}

std::string generateResponse(const std::string& userInput)
{
    const std::string lowerInput = toLowerCase(userInput);

    // Rule-based responses keep this step focused on C++ basics, not real AI.
    if (lowerInput.find("hello") != std::string::npos ||
        lowerInput.find("hi") != std::string::npos)
    {
        return "Hello! I am a small rule-based learning chatbot.";
    }

    if (lowerInput.find("name") != std::string::npos)
    {
        return "My name is Local Learning Bot.";
    }

    if (lowerInput.find("cpp") != std::string::npos ||
        lowerInput.find("c++") != std::string::npos)
    {
        return "C++ is a great language for learning structure and memory basics.";
    }

    if (lowerInput.find("help") != std::string::npos)
    {
        return "You can type /help to see the command list.";
    }

    if (lowerInput.find("bye") != std::string::npos)
    {
        return "Goodbye! Type /exit when you are ready to quit.";
    }

    return "I do not understand deeply yet, but I saved your message.";
}
