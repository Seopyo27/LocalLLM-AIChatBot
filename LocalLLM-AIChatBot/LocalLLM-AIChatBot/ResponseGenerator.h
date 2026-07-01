#pragma once

#include <map>
#include <string>

std::map<std::string, std::string> createDefaultResponses();
std::string generateResponse(const std::string& userInput, const std::map<std::string, std::string>& keywordResponses);
void printKeywordResponses(const std::map<std::string, std::string>& keywordResponses);
void teachResponse(std::map<std::string, std::string>& keywordResponses, const std::string& keyword, const std::string& response);
bool unteachResponse(std::map<std::string, std::string>& keywordResponses, const std::string& keyword);
