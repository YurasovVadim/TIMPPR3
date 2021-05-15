#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;
// Функции  нужна для обработки ошибок. Errtext - причина ошибки. Ex - код завершения ( по дефолту 1 )
void errHandle (const char * errtext, const int Ex = 1)
{
    cerr << errtext << endl;
    exit(Ex);
}

int main(int argc, char **argv)
{
    // I шаг.
    // Подготовка структуры с адресом клиента
    sockaddr_in * Add = new (sockaddr_in);
    Add->sin_family = AF_INET;      // интернет протокол IPv4
    Add->sin_port = 0;              // Любой порт на усмотрение ОС
    Add->sin_addr.s_addr = 0;       // все адреса компьютера

    // Подготовка структуры с адресом сервера
    sockaddr_in * Radd = new (sockaddr_in);
    Radd->sin_family = AF_INET; // интернет протокол IPv4
    Radd->sin_port = htons(7); // порт
    Radd->sin_addr.s_addr = inet_addr("82.179.90.12"); // локальный адрес сети

    // Подготовка буфера обмена для передачи и приема данных
    char *buf1 = new char [256];
    char *buf2 = new char [256];
    cout<<"Введите посылаемое сообщение: ";
    cin.getline (buf2, 256);
    strcpy(buf1, buf2); // Копируем строку
    int msgLen = strlen(buf1); // Вычисляем длину строки

    // II шаг.
    int Sock = socket(AF_INET, SOCK_DGRAM, 0); // Используем протокол UDP
    if (Sock == -1) {
        ("Ошибка открытия сокета", 11);
    }

    //III шаг.
    int rc = bind(Sock, (const sockaddr*) Add, sizeof (sockaddr_in));
    if (rc == -1) { // проверка
        close(Sock);
        ("Ошибка привязки сокета к своему адресу", 12);
    }

    // IV шаг.
    rc = connect(Sock, (const sockaddr*) Radd, sizeof(sockaddr_in));
    if (rc == -1) {
        close(Sock);
        ("Ошибка установки соединения с сервером", 13);
    }

    // V шаг.
    // Передача данных
    rc = send(Sock, buf1, msgLen, 0);
    if (rc == -1) { // проверка
        close(Sock);
        ("Ошибка отправки сообщения", 14);
    }
    cout << "Мы отправили: " << buf1 << endl;
    // Принятие ответа
    rc = recv(Sock, buf1, 256, 0);
    if (rc == -1) { // проверка
        close(Sock);
        errHandle("Ошибка получения ответа", 15);
    }
    buf1[rc]= '\0';
    std::cout<<"Мы получили: "<< buf1 << std::endl;

    // VI шаг.
    close(Sock);
    // Освобождение памяти
    delete Add;
    delete Radd;
    delete[] buf1;
    delete[] buf2;
    return 0;
}