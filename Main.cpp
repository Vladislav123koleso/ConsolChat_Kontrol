#include <iostream>
#include <string>
#include <list>
#include "mysql.h"
#include "Hello.h"
#include "User.h"
#include "Message.h"

using namespace std;

class Chat
{
    list<Message> messages; 

public:
    void sendMessage(MYSQL* mysql, const string& recipient, const string& sender, const string& text)
    {
        // ��������� ��������� � ���� ������
        string query = "INSERT INTO messages (recipient_id, sender_id, text) VALUES "
            "((SELECT id FROM users WHERE username = '" + recipient + "'), "
            "(SELECT id FROM users WHERE username = '" + sender + "'), '" + text + "')";

        if (mysql_query(mysql, query.c_str())) {
            cout << "Error: failed to insert message into database " << mysql_error(mysql) << endl;
        }
        else {
            Message message1(recipient, sender, text);
            messages.push_back(message1);
        }
    }

    void loadMessages(MYSQL* mysql, const string& currentUser)
    {
        // ������� ������ ��������� ����� ��������� �����
        messages.clear();

        // ��������� ��������� �� ���� ������ ��� �������� ������������
        string query = "SELECT sender.username, recipient.username, messages.text FROM messages "
            "JOIN users AS sender ON messages.sender_id = sender.id "
            "JOIN users AS recipient ON messages.recipient_id = recipient.id "
            "WHERE sender.username = '" + currentUser + "' OR recipient.username = '" + currentUser + "'";

        if (mysql_query(mysql, query.c_str())) {
            cout << "Error: failed to retrieve messages from database " << mysql_error(mysql) << endl;
            return;
        }

        MYSQL_RES* res = mysql_store_result(mysql);
        if (res == nullptr) {
            cout << "Error: failed to store result " << mysql_error(mysql) << endl;
            return;
        }

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res)) != NULL) {
            string sender = row[0];
            string recipient = row[1];
            string text = row[2];
            Message message(recipient, sender, text);
            messages.push_back(message);
        }

        mysql_free_result(res);
    }

    void displayMessages()
    {
        if (messages.empty()) {
            cout << "� ��� ��� ���������.\n";
            return;
        }

        for (const auto& message : messages)
        {
            cout << "��: " << message.getSend() << ", ����: " << message.getRecepient() << "\n���������: " << message.getText() << endl;
        }
    }
};

int main()
{
    MYSQL mysql;
    mysql_init(&mysql);

    system("chcp 1251");
    hello();

    if (&mysql == NULL) {
        cout << "Error: can't create MySQL-descriptor" << endl;
        return 1;
    }

    // ������������ � �������
    if (!mysql_real_connect(&mysql, "localhost", "root", "admin", "chat_skillfactory_mySQL", 0, NULL, 0)) {
        cout << "Error: can't connect to database " << mysql_error(&mysql) << endl;
        return 1;
    }
    else {
        cout << "Success!" << endl;
    }

    list<User> users;
    User* currentUser = nullptr;

    // �������� ������������� �� ���� ������
    if (mysql_query(&mysql, "SELECT username, password FROM users")) {
        cout << "Error: failed to retrieve users from database " << mysql_error(&mysql) << endl;
    }
    else {
        MYSQL_RES* res = mysql_store_result(&mysql);
        MYSQL_ROW row;

        while ((row = mysql_fetch_row(res))) {
            string username = row[0];
            string password = row[1];
            User user(username, password);
            users.push_back(user);
        }

        mysql_free_result(res);
    }

    string username, password;
    
    User first(username, password);


    int choice;
    Chat chat;

    do {
        cout << " \n";
        cout << "�������� ��������:\n ";
        cout << "1. ������� ���������\n";
        cout << "2. ����� � ������������ �������\n";
        cout << "3. ������� ����� �������\n";
        cout << "4. ����������� ���������\n";
        cout << "5. ����� �� ��������\n";
        cout << "6. ���������\n";
        cout << " \n";
        cout << "� ���������� ����������� ����� ��������" << endl;

        cin >> choice;

        if (choice < 1 || choice > 5) {
            cout << "�� ����� ������������ ��������. ����������, �������� �������� �� 1 �� 5." << endl;
            continue;
        }

        switch (choice)
        {
        case 1:
            if (currentUser != nullptr) {
                string recipient, text;
                cout << "������� ����������: ";
                cin >> recipient;
                cout << "������� ����� ���������: ";
                cin.ignore();
                getline(cin, text);

                chat.sendMessage(&mysql, recipient, currentUser->getName(), text);
                cout << "��������� ����������!\n";
            }
            else {
                cout << "�� �� ����� � �������. ������� ������� � ���� �������, ����� 2.\n";
            }
            break;

        case 2:
            cout << "���� � ������������ �������\n";
            {
                string username1, password1;
                cout << "������� �����: ";
                cin >> username1;
                cout << "������� ������: ";
                cin >> password1;

                // ��������� ����� � ������ ������������
                string query = "SELECT username, password FROM users WHERE username = '" + username1 + "' AND password = '" + password1 + "'";
                if (mysql_query(&mysql, query.c_str())) {
                    cout << "Error: failed to retrieve user from database " << mysql_error(&mysql) << endl;
                }
                else {
                    MYSQL_RES* res = mysql_store_result(&mysql);
                    if (mysql_num_rows(res) == 1) {
                        currentUser = &users.front(); // ������������� �������� ������������
                        cout << "�� ������� ����� � �������!" << endl;
                    }
                    else {
                        cout << "������������ ����� ��� ������." << endl;
                    }
                    mysql_free_result(res);
                }
            }
            break;

        case 3:
            cout << "�����������\n";
            {
                string newUsername, newPassword;
                cout << "������� �����: ";
                cin >> newUsername;
                cout << "������� ������: ";
                cin >> newPassword;

                // ���������, ���������� �� ������������ � ����� �������
                string query = "SELECT username FROM users WHERE username = '" + newUsername + "'";
                if (mysql_query(&mysql, query.c_str())) {
                    cout << "Error: failed to check username in database " << mysql_error(&mysql) << endl;
                }
                else {
                    MYSQL_RES* res = mysql_store_result(&mysql);
                    if (mysql_num_rows(res) == 0) {
                        // ��������� ������ ������������ � ���� ������
                        query = "INSERT INTO users (username, password) VALUES ('" + newUsername + "', '" + newPassword + "')";
                        if (mysql_query(&mysql, query.c_str())) {
                            cout << "Error: failed to insert user into database " << mysql_error(&mysql) << endl;
                        }
                        else {
                            User newUser(newUsername, newPassword);
                            users.push_back(newUser);
                            currentUser = &users.back();
                            cout << "�� ������� ������������������!" << endl;
                        }
                    }
                    else {
                        cout << "������������ � ����� ������� ��� ����������. �������� ������ �����." << endl;
                    }
                    mysql_free_result(res);
                }
            }
            break;

        case 4:
            if (currentUser != nullptr) {
                chat.loadMessages(&mysql, currentUser->getName());
                chat.displayMessages();
            }
            else {
                cout << "�� �� ����� � �������. ������� ������� � ���� �������, ����� 2.\n";
            }
            break;

        case 5:
            currentUser = nullptr;
            cout << "����� ��������. ������� � ������������ �������, ����� 2, ��� �����������������, ����� 3.\n";
            break;

        case 6:
            cout << "�� ��������� ������� ����� �� ����.\n";
            cout << "� ������� ��� ������ ����� ��������� ��� �������.\n";
            cout << "� ���� ��� ������ ������� ������.\n";
            break;

        default:
            cout << "�� ����� ������������ ��������.\n";
            break;
        }
    } while (choice != 5);

    // ��������� ���������� � ����� ������
    mysql_close(&mysql);

    return 0;
}
