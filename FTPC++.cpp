#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <Winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <ws2tcpip.h>
#include <ctime>
#include <Windows.h>
#include <string>
#include <thread>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;
char buff[1024];


void answer_from_Server(SOCKET s)
{
    bool flag = true;
    do
    {
        char buf[1024];
        ZeroMemory(buf, 1024);
        recv(s, buf, 1024, 0);
        cout << buf;
        if (strcmp(buf, "221 Goodbye.\r\n") == 0)flag = false;
    } while (flag);
}

void mess_to_Server(SOCKET s)
{
    char buf[1024];
    do
    {
        ZeroMemory(buf, 1024);
        cin.getline(buf, 1024);
        send(s, buf, 1024, 0);
    } while (strcmp(buf, "exit"));
}
void getdata(SOCKET s)
{
    while (true)
    {
        char file[1024];
        ZeroMemory(file, 1024);
        recv(s, file, sizeof(file), 0);
        cout << file;
    }
}


int main()
{
    setlocale(LC_ALL, "Russian");

    string add;
    int port = 21;
    int port2 = 20;

    WSADATA ws;
    WSAStartup(MAKEWORD(1, 1), &ws);
    SOCKET s1, s2;
    s1 = socket(AF_INET, SOCK_STREAM, 0);
    if (s1 == INVALID_SOCKET)
    {
        cout << stderr << "Ошибка создания сокета!" << endl;
        exit(1);
    }
    s2 = socket(AF_INET, SOCK_STREAM, 0);
    if (s2 == INVALID_SOCKET)
    {
        cout << stderr << "Ошибка создания сокета!" << endl;
        exit(1);
    }



    cout << "Введите адрес сервера" << endl;
    cin >> add;

    //управляющее соединение
    sockaddr_in addres;
    ZeroMemory(&addres, sizeof(addres));
    addres.sin_family = AF_INET;
    addres.sin_addr.S_un.S_addr = inet_addr(add.c_str());
    addres.sin_port = htons(u_short(port));

   int flag1 = connect(s1, (sockaddr*)&addres, sizeof(addres));


    //Создание соединения данных

    sockaddr_in addresdata;
    ZeroMemory(&addresdata, sizeof(addresdata));
    addresdata.sin_family = AF_INET;
    addresdata.sin_addr.S_un.S_addr = INADDR_ANY;
    addresdata.sin_port = htons(1800);

    int flag2 = bind(s2, (sockaddr*)&addresdata, sizeof(addresdata));
    int error = WSAGetLastError();
    cout << error << endl;
    listen(s2, 1);
    //поток для получения ответов от управляющего соединения
    thread answers(answer_from_Server, s1);
    //Поток для получения ответов от соединения данных
    thread answersdata(getdata, s2);

    //Поток для отправки сообщений
   // thread messages(mess_to_Server, s1);

    //Вход на сервер
   
    send(s1, "USER ftp", 9, 0);
    send(s1, "PASS ftp", 9, 0);
    send(s1, "PORT 127,0,0,1,7,8", 19, 0);
    send(s1, "TYPE A", 7, 0);


    send(s1, "LIST", 5, 0);
    

    answers.join();
    answersdata.join();
 
}
