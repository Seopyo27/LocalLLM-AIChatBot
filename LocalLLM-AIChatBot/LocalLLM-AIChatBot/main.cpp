#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "ChatHistory.h"
#include "Message.h"
#include "ResponseGenerator.h"

void printWelcome()
{
    std::cout << "Local Learning Chatbot - Step 1\n";
    std::cout << "Type /help to see commands, or /exit to quit.\n\n";
}

void printHelp()
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

std::string trimLeadingSpaces(const std::string& text)
{
    std::string result = text;

    while (!result.empty() && result[0] == ' ')
    {
        result.erase(0, 1);
    }

    return result;
}

bool startsWithCommand(const std::string& input, const std::string& command)
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

std::string getCommandArgument(const std::string& input, const std::string& command)
{
    if (input.size() <= command.size())
    {
        return "";
    }

    return trimLeadingSpaces(input.substr(command.size()));
}

bool parseTeachCommand(const std::string& input, std::string& keyword, std::string& response)
{
    std::istringstream inputStream(input);
    std::string command;

    inputStream >> command;
    inputStream >> keyword;

    std::getline(inputStream, response);
    response = trimLeadingSpaces(response);

    return !keyword.empty() && !response.empty();
}

int main()
{
    std::vector<Message> history;
    std::map<std::string, std::string> keywordResponses = createDefaultResponses();
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
            printHistory(history);
            continue;
        }

        if (startsWithCommand(userInput, "/find"))
        {
            const std::string keyword = getCommandArgument(userInput, "/find");
            findMessages(history, keyword);
            continue;
        }

        if (userInput == "/keywords")
        {
            printKeywordResponses(keywordResponses);
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

            teachResponse(keywordResponses, keyword, response);
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

            if (unteachResponse(keywordResponses, keyword))
            {
                std::cout << "Deleted response for keyword \"" << keyword << "\".\n\n";
            }
            else
            {
                std::cout << "No response found for keyword \"" << keyword << "\".\n\n";
            }

            continue;
        }

        addMessage(history, "User", userInput);

        const std::string botResponse = generateResponse(userInput, keywordResponses);
        addMessage(history, "Bot", botResponse);

        std::cout << "Bot: " << botResponse << "\n\n";
    }

    return 0;
}
