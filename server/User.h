#pragma once
#include <string>

class User
{
    std::string _login;
    std::string _password;

public:
    User();
    User(const std::string& login, const std::string& password);

    const std::string getUserLogin() const;
    const std::string getUserPassword() const;

    friend std::istream& operator >> (std::istream&, User&);
    friend std::ostream& operator << (std::ostream&, const User&);

    ~User() = default;
};

