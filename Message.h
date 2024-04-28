#pragma once
#include<string>


class Message
{
private:
    std::string recepient_p; // ����������
    std::string send_p;      // �����������
    std::string text_p;      // ����� ���������

public:
    // ������������
    Message() : recepient_p(""), send_p(""), text_p("") {}
    Message(const std::string& recepient, const std::string& send, const std::string& text)
        : recepient_p(recepient), send_p(send), text_p(text) {}

    // ������� ��� ��������� �������� �����
    const std::string& getRecepient() const { return recepient_p; }
    const std::string& getSend() const { return send_p; }
    const std::string& getText() const { return text_p; }
};