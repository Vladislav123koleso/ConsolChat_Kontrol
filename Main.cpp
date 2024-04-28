#include<iostream>
#include<string>
#include <list>
#include"Hello.h"
#include"User.h"
#include"Message.h"
using namespace std;


//массив сообщений 
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
            cout << "В чате есть сообщение!!! Кому:  " << data[f].getRecepient() << ". От кого: " << data[f].getSend() << endl;
            cout << "cообщение: " << "'" << data[f].getText() << "'" << endl;
        };

    };


};






//массив юзеров
template<typename U>
class MemoryUser
{
private:
    std::list<User> users;

public:
    // функция для добавления нового пользователя
    void addUser(const User& user) {
        users.push_back(user);
    }

    // функция для получения пользователя по индексу
    User* getUser(int index) {
        if (index < 0 || index >= users.size()) {
            return nullptr;
        }
        auto it = users.begin();
        std::advance(it, index);
        return &(*it);
    }

    // функция для получения количества пользователей
    int getSize() {
        return users.size();
    }
};




class Chat
{
    list<Message> messages; // Заменяем Memory<Message> на std::list<Message>
public:
    void sendMessage(const string& recepient, const string& send, const string& text)
    {
        Message message1(recepient, send, text);
        messages.push_back(message1); // Используем push_back для добавления сообщения в конец списка
    };

    void displayMessages()
    {
        for (const auto& message : messages)
        {
            cout << "В чате есть сообщение!!! Кому:  " << message.getRecepient() << ". От кого: " << message.getSend() << endl;
            cout << "cообщение: " << "'" << message.getText() << "'" << endl;
        }
    };
};





int main()
{
    system("chcp 1251");
	hello();


    list<User> users; // Используем std::list для хранения пользователей
    const User* currentUser = nullptr;


    cout << "Регистрация\n";
    string username, password;
    cout << "Введите логин: " << endl;
    cin >> username;
    cout << "Введите пароль: " << endl;
    cin >> password;
    User First(username, password);
    users.push_back(First); // Добавляем пользователя в конец списка пользователей
    cout << "Вы успешно зарегистрировались!Теперь войдите в аккаунт." << endl;

    cout << " \n";
    cout << "Выберите действие:\n ";

    cout << "1. Послать сообщение\n";

    cout << "2. Войти в существующий аккаунт\n";

    cout << "3. Создать новый аккаунт\n";

    cout << "4. Выход из аккаунта\n";

    cout << "5. Завершить\n";

    cout << " \n";

    cout << "В дальнейшем пользуйтесь этими кнопками" << endl;
    int choice;

    Chat chat;
    do {
        cin >> choice;

        if (choice < 1 || choice > 5) {
            cout << "Вы ввели неправильное значение. Пожалуйста, выберите действие от 1 до 5." << endl;
            continue; // Переходим к следующей итерации цикла
        }

        switch (choice)
        {
            {case 1:
                if (currentUser != nullptr) { // только если пользователь вошел
                    string recipient, text;
                    cout << "Введите получателя: ";
                    cin >> recipient;
                    cout << "Введите текст сообщения: ";
                    cin.ignore();
                    getline(cin, text);

                    chat.sendMessage(recipient, currentUser->getName(), text);
                    cout << "Сообщение отправлено!\n";
                    cout << " \n";
                    cout << " \n";
                }
                else {
                    cout << "Вы не вошли в систему. Сначала войдите в свой аккаунт нажав 2.\n";
                }
                break;
            }


            {case 2:
                cout << "Вход в существующий аккаунт\n";
                string username1, password1;
                cout << "Введите логин: ";
                cin >> username1;
                cout << "Введите пароль: ";
                cin >> password1;

                // Проходим по списку пользователей и ищем пользователя с введенным логином и паролем
                bool found = false;
                for (const auto& user : users) {
                    if (user.getName() == username1 && user.getPassword() == password1) {
                        currentUser = &user;
                        found = true;
                        break;
                    }
                }

                // Если пользователь найден, выводим сообщения чата
                if (found) {
                    cout << "Вы успешно вошли в аккаунт!" << endl;
                    // Добавьте здесь код для отображения сообщений чата, если необходимо
                }
                else {
                    cout << "Неправильный логин или пароль." << endl;
                }
                break;
            }



            {case 3:
                cout << "Регистрация\n";
                string newUsername, newPassword;
                cout << "Введите логин: ";
                cin >> newUsername;
                cout << "Введите пароль: ";
                cin >> newPassword;

                // Проверяем, существует ли пользователь с таким логином
                bool userExists = false;
                for (const auto& user : users) {
                    if (user.getName() == newUsername) {
                        userExists = true;
                        break;
                    }
                }

                if (!userExists) {
                    // Создаем нового пользователя и добавляем его в список
                    User newUser(newUsername, newPassword);
                    users.push_back(newUser);
                    currentUser = &users.back(); // Устанавливаем currentUser на только что созданного пользователя
                    cout << "Вы успешно зарегистрировались!" << endl;
                }
                else {
                    cout << "Пользователь с таким логином уже существует. Выберите другой логин." << endl;
                }
                break;
            }

            {case 4:
                currentUser = nullptr; // Устанавливаем currentUser в nullptr при выходе
                cout << "Выход выполнен. Войдите в существующий аккаунт нажав 2 или зарегестрируйтесь нажав 3\n";
                cout << " \n";
                cout << " \n";
                break;
            }

            {case 5:

                cout << "Вы выполнили учебный выход из чата\n";
                cout << "В будущем эта кнопка будет закрывть всю консоль\n";
                cout << "А пока - это только учебный проект" << endl;
                break;
            }



        default:
            cout << "Вы ввели неправильное значение" << endl;
            break;

        };//////

    } while (choice != 5 );

	return 0;
}


