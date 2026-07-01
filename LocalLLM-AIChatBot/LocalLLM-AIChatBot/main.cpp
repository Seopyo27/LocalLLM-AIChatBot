#include <cctype>
#include <iostream>
#include <string>
#include <vector>

// A struct groups related data without adding advanced class behavior yet.
struct Message
{
    std::string role;
    std::string content;
};

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
