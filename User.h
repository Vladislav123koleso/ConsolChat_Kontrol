#pragma once
#include<string>
using namespace std;

class User
{
private:
    string name_p; //имя
    string password_p;//пароль

public:

    User()//
    {
        name_p = " ";
        password_p = " ";
    };

    //Конструктор быстрого заполнения
    User(const string& name, const string& password) :name_p(name), password_p(password)
    {};

    //геттеры
    string& getName()
    {
        return name_p;
    };

    string& getPassword()
    {
        return password_p;
    };
};