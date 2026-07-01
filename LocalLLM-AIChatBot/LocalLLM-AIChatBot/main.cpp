#include <iostream>
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
    std::cout << "  /exit    Quit the program\n\n";
}

int main()
{
    std::vector<Message> history;
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

        addMessage(history, "User", userInput);

        const std::string botResponse = generateResponse(userInput);
        addMessage(history, "Bot", botResponse);

        std::cout << "Bot: " << botResponse << "\n\n";
    }

    return 0;
}
