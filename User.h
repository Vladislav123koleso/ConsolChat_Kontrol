#pragma once
#include<string>
using namespace std;

class User
{
private:
    std::string name_p;     // Имя пользователя
    std::string password_p; // Пароль пользователя

public:
    // Конструкторы
    User() : name_p(""), password_p("") {}
    User(const std::string& name, const std::string& password)
        : name_p(name), password_p(password) {}

    // Геттеры для получения значений полей
    const std::string& getName() const { return name_p; }
    const std::string& getPassword() const { return password_p; }
};