#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_BOOKS 100
#define MAX_BUFFER_SIZE 1024

struct Book {
    char identifier[50];
    int row;
    int shelf;
    int book_number;
};

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Usage: %s <ip_address> <port> <row>\n", argv[0]);
        return 1;
    }

    const char* ip = argv[1];
    int port = atoi(argv[2]);
    int row = atoi(argv[3]);

    struct sockaddr_in server_addr;
    int client_fd;
    char buffer[MAX_BUFFER_SIZE];

    struct Book books[MAX_BOOKS];
    int num_books = 0;

    // Создание TCP сокета
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        perror("Socket creation error");
        return 1;
    }

    // Настройка сервера
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    server_addr.sin_port = htons(port);

    // Установка соединения с сервером
    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection error");
        return 1;
    }

    // Ввод данных о книгах для подкаталога
    printf("Enter book details (identifier, shelf, book number):\n");

    while (1) {
        if (num_books >= MAX_BOOKS) {
            printf("Maximum book count reached\n");
            break;
        }

        struct Book* book = &books[num_books];
        (*book).row = row;
        printf("Book %d:\n", num_books + 1);
        printf("Identifier: ");
        fgets(book->identifier, sizeof(book->identifier), stdin);
        book->identifier[strcspn(book->identifier, "\n")] = '\0';

        printf("Shelf: ");
        scanf("%d", &book->shelf);

        printf("Book Number: ");
        scanf("%d", &book->book_number);

        getchar(); // Считывание символа новой строки

        num_books++;

        printf("Enter more books? (y/n): ");
        char choice;
        scanf("%c", &choice);

        getchar(); // Считывание символа новой строки

        if (choice == 'n' || choice == 'N')
            break;
    }

    // Отправка данных о книгах на сервер
    ssize_t num_bytes = send(client_fd, (void*)books, num_books * sizeof(struct Book), 0);
    if (num_bytes <= 0) {
        perror("Sending error");
        return 1;
    }

    printf("Subcatalog sent to server\n");

    // Закрытие соединения с сервером
    close(client_fd);

    return 0;
}
