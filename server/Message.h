﻿#pragma once
#include <string>

class Message
{
    std::string _from;
    std::string _to;
    std::string _text;

public:
    Message();
    Message(const std::string& from, const std::string& to, const std::string& text);

    const std::string getFrom() const;
    const std::string getTo() const;
    const std::string getText() const;

    friend std::istream& operator >> (std::istream&, Message&);
    friend std::ostream& operator << (std::ostream&, const Message&);

    ~Message() = default;
};


