#include "Message.h"

Message::Message(const std::string& role, const std::string& content)
    : role_(role), content_(content)
{
}

Message::~Message() = default;

const std::string& Message::getRole() const
{
    return role_;
}

const std::string& Message::getContent() const
{
    return content_;
}
