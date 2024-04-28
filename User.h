#pragma once
#include<string>
using namespace std;

class User
{
private:
    std::string name_p;     // ��� ������������
    std::string password_p; // ������ ������������

public:
    // ������������
    User() : name_p(""), password_p("") {}
    User(const std::string& name, const std::string& password)
        : name_p(name), password_p(password) {}

    // ������� ��� ��������� �������� �����
    const std::string& getName() const { return name_p; }
    const std::string& getPassword() const { return password_p; }
};