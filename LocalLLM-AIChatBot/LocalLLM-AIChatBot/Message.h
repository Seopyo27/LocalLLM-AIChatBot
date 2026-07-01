#pragma once

#include <string>

class Message
{
public:
    Message(const std::string& role, const std::string& content);
    ~Message();

    const std::string& getRole() const;
    const std::string& getContent() const;

private:
    std::string role_;
    std::string content_;
};
