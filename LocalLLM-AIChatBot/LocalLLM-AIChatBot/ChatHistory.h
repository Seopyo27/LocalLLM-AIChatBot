#pragma once

#include <string>
#include <vector>

#include "Message.h"

void addMessage(std::vector<Message>& history, const std::string& role, const std::string& content);
void printHistory(const std::vector<Message>& history);
