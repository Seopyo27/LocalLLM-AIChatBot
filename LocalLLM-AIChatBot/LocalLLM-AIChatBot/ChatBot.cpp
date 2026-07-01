#include "ChatBot.h"

#include <iostream>
#include <sstream>

#include "ResponseGenerator.h"

ChatBot::ChatBot()
    : history_(std::make_unique<ChatHistory>()),
      keywordResponses_(createDefaultResponses()),
      localLLMClient_()
{
}

ChatBot::~ChatBot() = default;

void ChatBot::printWelcome() const
{
    std::cout << "Local Learning Chatbot - Step 1\n";
    std::cout << "Type /help to see commands, or /exit to quit.\n\n";
}

void ChatBot::printHelp() const
{
    std::cout << "Commands:\n";
    std::cout << "  /help    Show this help message\n";
    std::cout << "  /history Show the conversation history\n";
    std::cout << "  /find <keyword> Search conversation history\n";
    std::cout << "  /keywords Show keyword responses\n";
    std::cout << "  /teach <keyword> <response> Add or update a keyword response\n";
    std::cout << "  /unteach <keyword> Delete a keyword response\n";
    std::cout << "  /exit    Quit the program\n\n";
}

std::string ChatBot::trimLeadingSpaces(const std::string& text) const
{
    std::string result = text;

    while (!result.empty() && result[0] == ' ')
    {
        result.erase(0, 1);
    }

    return result;
}

bool ChatBot::startsWithCommand(const std::string& input, const std::string& command) const
{
    if (input == command)
    {
        return true;
    }

    if (input.size() <= command.size())
    {
        return false;
    }

    return input.substr(0, command.size()) == command && input[command.size()] == ' ';
}

std::string ChatBot::getCommandArgument(const std::string& input, const std::string& command) const
{
    if (input.size() <= command.size())
    {
        return "";
    }

    return trimLeadingSpaces(input.substr(command.size()));
}

bool ChatBot::parseTeachCommand(const std::string& input, std::string& keyword, std::string& response) const
{
    std::istringstream inputStream(input);
    std::string command;

    inputStream >> command;
    inputStream >> keyword;

    std::getline(inputStream, response);
    response = trimLeadingSpaces(response);

    return !keyword.empty() && !response.empty();
}

void ChatBot::run()
{
    std::string userInput;

    printWelcome();

    while (true)
    {
        std::cout << "You: ";
        std::getline(std::cin, userInput);

        if (userInput.empty())
        {
            continue;
        }

        if (userInput == "/exit")
        {
            std::cout << "Bot: Goodbye!\n";
            break;
        }

        if (userInput == "/help")
        {
            printHelp();
            continue;
        }

        if (userInput == "/history")
        {
            history_->printHistory();
            continue;
        }

        if (startsWithCommand(userInput, "/find"))
        {
            const std::string keyword = getCommandArgument(userInput, "/find");
            history_->findMessages(keyword);
            continue;
        }

        if (userInput == "/keywords")
        {
            printKeywordResponses(keywordResponses_);
            continue;
        }

        if (startsWithCommand(userInput, "/teach"))
        {
            std::string keyword;
            std::string response;

            if (!parseTeachCommand(userInput, keyword, response))
            {
                std::cout << "Usage: /teach <keyword> <response>\n\n";
                continue;
            }

            teachResponse(keywordResponses_, keyword, response);
            std::cout << "Learned response for keyword \"" << keyword << "\".\n\n";
            continue;
        }

        if (startsWithCommand(userInput, "/unteach"))
        {
            const std::string keyword = getCommandArgument(userInput, "/unteach");

            if (keyword.empty())
            {
                std::cout << "Usage: /unteach <keyword>\n\n";
                continue;
            }

            if (unteachResponse(keywordResponses_, keyword))
            {
                std::cout << "Deleted response for keyword \"" << keyword << "\".\n\n";
            }
            else
            {
                std::cout << "No response found for keyword \"" << keyword << "\".\n\n";
            }

            continue;
        }

        history_->addMessage("User", userInput);

        std::string botResponse;

        if (!tryGenerateKeywordResponse(userInput, keywordResponses_, botResponse))
        {
            // If no keyword matches, ask the local Ollama API for a response.
            if (!localLLMClient_.generateResponse(userInput, botResponse))
            {
                botResponse = "Local LLM response could not be fetched. The message was saved with the existing flow.";
            }
        }

        history_->addMessage("Bot", botResponse);

        std::cout << "Bot: " << botResponse << "\n\n";
    }
}
