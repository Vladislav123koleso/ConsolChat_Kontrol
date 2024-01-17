#pragma once
#include<string>


class Message
{
private:
    string recepient_p; /*кому*/
    string send_p;/* кто*/
    string text_p; /*текст смс*/
public:

    Message()
    {
        recepient_p = " ";
        send_p = " ";
        text_p = " ";
    }

    Message(const string& recepient, const string& send, const string& text) :recepient_p(recepient), send_p(send), text_p(text)
    {};
    string& getRecepient()
    {
        return recepient_p;//кто
    };

    string& getSend()//кому
    {
        return send_p;
    };

    string& getText()//что
    {
        return text_p;
    };
};