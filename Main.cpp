#include<iostream>
#include<string>
#include <list>
#include"Hello.h"
#include"User.h"
#include"Message.h"
using namespace std;


//������ ��������� 
template<typename T>
class Memory {
private:
    int size;
    T* data;
    int i;
public:
    Memory()
    {
        size = 1;
        data = new T[size];
        i = 0;
    };
    ~Memory()
    {
        delete[] data;
    };

    void AddSms(const T& value)
    {
        if (i < size)
        {
            data[i] = value;
            ++i;
        }
        else
        {
            int newSize = size * 2;
            T* newData = new T[newSize];
            for (int j = 0; j < size; j++)
            {
                newData[j] = data[j];
            }

            newData[i] = value;
            ++i;
            delete[] data;
            data = newData;
            size = newSize;
        };
    };

    void Display()
    {
        for (int f = 0; f < i; f++)
        {
            cout << "� ���� ���� ���������!!! ����:  " << data[f].getRecepient() << ". �� ����: " << data[f].getSend() << endl;
            cout << "c��������: " << "'" << data[f].getText() << "'" << endl;
        };

    };


};






//������ ������
template<typename U>
class MemoryUser
{
private:
    std::list<User> users;

public:
    // ������� ��� ���������� ������ ������������
    void addUser(const User& user) {
        users.push_back(user);
    }

    // ������� ��� ��������� ������������ �� �������
    User* getUser(int index) {
        if (index < 0 || index >= users.size()) {
            return nullptr;
        }
        auto it = users.begin();
        std::advance(it, index);
        return &(*it);
    }

    // ������� ��� ��������� ���������� �������������
    int getSize() {
        return users.size();
    }
};




class Chat
{
    list<Message> messages; // �������� Memory<Message> �� std::list<Message>
public:
    void sendMessage(const string& recepient, const string& send, const string& text)
    {
        Message message1(recepient, send, text);
        messages.push_back(message1); // ���������� push_back ��� ���������� ��������� � ����� ������
    };

    void displayMessages()
    {
        for (const auto& message : messages)
        {
            cout << "� ���� ���� ���������!!! ����:  " << message.getRecepient() << ". �� ����: " << message.getSend() << endl;
            cout << "c��������: " << "'" << message.getText() << "'" << endl;
        }
    };
};





int main()
{
    system("chcp 1251");
	hello();


    list<User> users; // ���������� std::list ��� �������� �������������
    const User* currentUser = nullptr;


    cout << "�����������\n";
    string username, password;
    cout << "������� �����: " << endl;
    cin >> username;
    cout << "������� ������: " << endl;
    cin >> password;
    User First(username, password);
    users.push_back(First); // ��������� ������������ � ����� ������ �������������
    cout << "�� ������� ������������������!������ ������� � �������." << endl;

    cout << " \n";
    cout << "�������� ��������:\n ";

    cout << "1. ������� ���������\n";

    cout << "2. ����� � ������������ �������\n";

    cout << "3. ������� ����� �������\n";

    cout << "4. ����� �� ��������\n";

    cout << "5. ���������\n";

    cout << " \n";

    cout << "� ���������� ����������� ����� ��������" << endl;
    int choice;

    Chat chat;
    do {
        cin >> choice;

        if (choice < 1 || choice > 5) {
            cout << "�� ����� ������������ ��������. ����������, �������� �������� �� 1 �� 5." << endl;
            continue; // ��������� � ��������� �������� �����
        }

        switch (choice)
        {
            {case 1:
                if (currentUser != nullptr) { // ������ ���� ������������ �����
                    string recipient, text;
                    cout << "������� ����������: ";
                    cin >> recipient;
                    cout << "������� ����� ���������: ";
                    cin.ignore();
                    getline(cin, text);

                    chat.sendMessage(recipient, currentUser->getName(), text);
                    cout << "��������� ����������!\n";
                    cout << " \n";
                    cout << " \n";
                }
                else {
                    cout << "�� �� ����� � �������. ������� ������� � ���� ������� ����� 2.\n";
                }
                break;
            }


            {case 2:
                cout << "���� � ������������ �������\n";
                string username1, password1;
                cout << "������� �����: ";
                cin >> username1;
                cout << "������� ������: ";
                cin >> password1;

                // �������� �� ������ ������������� � ���� ������������ � ��������� ������� � �������
                bool found = false;
                for (const auto& user : users) {
                    if (user.getName() == username1 && user.getPassword() == password1) {
                        currentUser = &user;
                        found = true;
                        break;
                    }
                }

                // ���� ������������ ������, ������� ��������� ����
                if (found) {
                    cout << "�� ������� ����� � �������!" << endl;
                    // �������� ����� ��� ��� ����������� ��������� ����, ���� ����������
                }
                else {
                    cout << "������������ ����� ��� ������." << endl;
                }
                break;
            }



            {case 3:
                cout << "�����������\n";
                string newUsername, newPassword;
                cout << "������� �����: ";
                cin >> newUsername;
                cout << "������� ������: ";
                cin >> newPassword;

                // ���������, ���������� �� ������������ � ����� �������
                bool userExists = false;
                for (const auto& user : users) {
                    if (user.getName() == newUsername) {
                        userExists = true;
                        break;
                    }
                }

                if (!userExists) {
                    // ������� ������ ������������ � ��������� ��� � ������
                    User newUser(newUsername, newPassword);
                    users.push_back(newUser);
                    currentUser = &users.back(); // ������������� currentUser �� ������ ��� ���������� ������������
                    cout << "�� ������� ������������������!" << endl;
                }
                else {
                    cout << "������������ � ����� ������� ��� ����������. �������� ������ �����." << endl;
                }
                break;
            }

            {case 4:
                currentUser = nullptr; // ������������� currentUser � nullptr ��� ������
                cout << "����� ��������. ������� � ������������ ������� ����� 2 ��� ����������������� ����� 3\n";
                cout << " \n";
                cout << " \n";
                break;
            }

            {case 5:

                cout << "�� ��������� ������� ����� �� ����\n";
                cout << "� ������� ��� ������ ����� �������� ��� �������\n";
                cout << "� ���� - ��� ������ ������� ������" << endl;
                break;
            }



        default:
            cout << "�� ����� ������������ ��������" << endl;
            break;

        };//////

    } while (choice != 5 );

	return 0;
}


