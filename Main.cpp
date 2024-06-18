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
        // Сохраняем сообщение в базе данных
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
        // Очищаем список сообщений перед загрузкой новых
        messages.clear();

        // Загружаем сообщения из базы данных для текущего пользователя
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
            cout << "У вас нет сообщений.\n";
            return;
        }

        for (const auto& message : messages)
        {
            cout << "От: " << message.getSend() << ", Кому: " << message.getRecepient() << "\nСообщение: " << message.getText() << endl;
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

    // Подключаемся к серверу
    if (!mysql_real_connect(&mysql, "localhost", "root", "admin", "chat_skillfactory_mySQL", 0, NULL, 0)) {
        cout << "Error: can't connect to database " << mysql_error(&mysql) << endl;
        return 1;
    }
    else {
        cout << "Success!" << endl;
    }

    list<User> users;
    User* currentUser = nullptr;

    // Загрузка пользователей из базы данных
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
        cout << "Выберите действие:\n ";
        cout << "1. Послать сообщение\n";
        cout << "2. Войти в существующий аккаунт\n";
        cout << "3. Создать новый аккаунт\n";
        cout << "4. Просмотреть переписку\n";
        cout << "5. Выход из аккаунта\n";
        cout << "6. Завершить\n";
        cout << " \n";
        cout << "В дальнейшем пользуйтесь этими кнопками" << endl;

        cin >> choice;

        if (choice < 1 || choice > 5) {
            cout << "Вы ввели неправильное значение. Пожалуйста, выберите действие от 1 до 5." << endl;
            continue;
        }

        switch (choice)
        {
        case 1:
            if (currentUser != nullptr) {
                string recipient, text;
                cout << "Введите получателя: ";
                cin >> recipient;
                cout << "Введите текст сообщения: ";
                cin.ignore();
                getline(cin, text);

                chat.sendMessage(&mysql, recipient, currentUser->getName(), text);
                cout << "Сообщение отправлено!\n";
            }
            else {
                cout << "Вы не вошли в систему. Сначала войдите в свой аккаунт, нажав 2.\n";
            }
            break;

        case 2:
            cout << "Вход в существующий аккаунт\n";
            {
                string username1, password1;
                cout << "Введите логин: ";
                cin >> username1;
                cout << "Введите пароль: ";
                cin >> password1;

                // Проверяем логин и пароль пользователя
                string query = "SELECT username, password FROM users WHERE username = '" + username1 + "' AND password = '" + password1 + "'";
                if (mysql_query(&mysql, query.c_str())) {
                    cout << "Error: failed to retrieve user from database " << mysql_error(&mysql) << endl;
                }
                else {
                    MYSQL_RES* res = mysql_store_result(&mysql);
                    if (mysql_num_rows(res) == 1) {
                        currentUser = &users.front(); // Устанавливаем текущего пользователя
                        cout << "Вы успешно вошли в аккаунт!" << endl;
                    }
                    else {
                        cout << "Неправильный логин или пароль." << endl;
                    }
                    mysql_free_result(res);
                }
            }
            break;

        case 3:
            cout << "Регистрация\n";
            {
                string newUsername, newPassword;
                cout << "Введите логин: ";
                cin >> newUsername;
                cout << "Введите пароль: ";
                cin >> newPassword;

                // Проверяем, существует ли пользователь с таким логином
                string query = "SELECT username FROM users WHERE username = '" + newUsername + "'";
                if (mysql_query(&mysql, query.c_str())) {
                    cout << "Error: failed to check username in database " << mysql_error(&mysql) << endl;
                }
                else {
                    MYSQL_RES* res = mysql_store_result(&mysql);
                    if (mysql_num_rows(res) == 0) {
                        // Сохраняем нового пользователя в базе данных
                        query = "INSERT INTO users (username, password) VALUES ('" + newUsername + "', '" + newPassword + "')";
                        if (mysql_query(&mysql, query.c_str())) {
                            cout << "Error: failed to insert user into database " << mysql_error(&mysql) << endl;
                        }
                        else {
                            User newUser(newUsername, newPassword);
                            users.push_back(newUser);
                            currentUser = &users.back();
                            cout << "Вы успешно зарегистрировались!" << endl;
                        }
                    }
                    else {
                        cout << "Пользователь с таким логином уже существует. Выберите другой логин." << endl;
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
                cout << "Вы не вошли в систему. Сначала войдите в свой аккаунт, нажав 2.\n";
            }
            break;

        case 5:
            currentUser = nullptr;
            cout << "Выход выполнен. Войдите в существующий аккаунт, нажав 2, или зарегистрируйтесь, нажав 3.\n";
            break;

        case 6:
            cout << "Вы выполнили учебный выход из чата.\n";
            cout << "В будущем эта кнопка будет закрывать всю консоль.\n";
            cout << "А пока это только учебный проект.\n";
            break;

        default:
            cout << "Вы ввели неправильное значение.\n";
            break;
        }
    } while (choice != 5);

    // Закрываем соединение с базой данных
    mysql_close(&mysql);

    return 0;
}
