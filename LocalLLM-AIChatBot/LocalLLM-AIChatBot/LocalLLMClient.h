#pragma once

#include <string>

// Calls the local Ollama HTTP API and returns false when the call is not available.
class LocalLLMClient
{
public:
    LocalLLMClient();
    ~LocalLLMClient();

    bool generateResponse(const std::string& prompt, std::string& response) const;
};
