#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;
// Функци ErrHandle нужна для обработки ошибок. Errtext - причина ошибки. Ex - код завершения ( по дефолту 1 )
void ErrHandle (const char * errtext, const int Ex = 1)
{
    cerr << errtext << endl;
    exit(Ex);
}

int main(int argc, char **argv)
{
    //I шаг.
    // Подготовка структуры с адресом клиента
    sockaddr_in * Add = new (sockaddr_in);
    Add->sin_family = AF_INET;      // Интернет-протокол IPv4
    Add->sin_port = 0;              // Любой порт
    Add->sin_addr.s_addr = 0;       // все адреса компьютера

    // Подготовка структуры с адресом сервера
    sockaddr_in * remAdd = new (sockaddr_in);
    remAdd->sin_family = AF_INET; // Интернет-протокол IPv4
    remAdd->sin_port = htons(13); // Ипользуемый порт
    remAdd->sin_addr.s_addr = inet_addr("82.179.90.12"); // Локальный адрес сети

    // Подготовка буфера обмена для передачи и приема данных
    char *count = new char [256];
    strcpy(count, "Определите дату и время\n"); // Копируем строку
    int msgLen = strlen(count);             // Вычисляем длину строки

    // II шаг.
    int MS = socket(AF_INET, SOCK_STREAM, 0); // Используем протокол TCP
    if (MS == -1) {
        ErrHandle("Ошибка создания сокета", 11);
    }
    
    //III шаг.
    int rc = bind(MS, (const sockaddr*) Add, sizeof (sockaddr_in));
    if (rc == -1) { // проверка
        close(MS);
        ErrHandle("Ошибка привязки сокета с адресом компьютера", 12);
    }
    
    // IV шаг.
    rc = connect(MS, (const sockaddr*) remAdd, sizeof(sockaddr_in));
    if (rc == -1) {
        close(MS);
        ErrHandle("Ошибка соединения с сервером", 13);
    }
    
    // V шаг.
    // Передача данных
    rc = send(MS, count, msgLen, 0);
    if (rc == -1) { // проверка
        close(MS);
        ErrHandle("Ошибка отправки запроса", 14);
    }
    cout << "Мы отправили: " << count << endl;
    // Принятие ответа
    rc = recv(MS, count, 256, 0);
    if (rc == -1) { // проверка
        close(MS);
        ErrHandle("Ошибка получения ответа", 15);
    }
    count[rc]= '\0';
    std::cout<<"Мы получили: "<< count << std::endl;
    
    // VI шаг.
    close(MS);
    // Освобождение памяти
    delete Add;
    delete remAdd;
    delete[] count;
    return 0;
}