#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_ROWS 100
#define MAX_BOOKS 100
#define MAX_BUFFER_SIZE 1024

struct Book {
    char identifier[50];
    int row;
    int shelf;
    int book_number;
};

struct Catalog {
    struct Book books[MAX_ROWS][MAX_BOOKS];
    int book_count[MAX_ROWS];
};

void sort_catalog(struct Catalog* catalog, int total_rows) {
    // Сортировка каталога по алфавиту (по идентификатору книги)
    for (int i = 0; i < total_rows; i++) {
        int book_count = catalog->book_count[i];

        for (int j = 0; j < book_count - 1; j++) {
            for (int k = 0; k < book_count - j - 1; k++) {
                if (strcmp(catalog->books[i][k].identifier, catalog->books[i][k + 1].identifier) > 0) {
                    // Обмен местами
                    struct Book temp = catalog->books[i][k];
                    catalog->books[i][k] = catalog->books[i][k + 1];
                    catalog->books[i][k + 1] = temp;
                }
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Usage: %s <ip_address> <port> <rows>\n", argv[0]);
        return 1;
    }

    const char* ip = argv[1];
    int port = atoi(argv[2]);
    int rows = atoi(argv[3]);
    struct sockaddr_in server_addr, client_addr;
    int server_fd, client_fd;
    socklen_t client_len;
    char buffer[MAX_BUFFER_SIZE];

    struct Catalog catalog;
    memset(&catalog, 0, sizeof(struct Catalog));

    // Создание TCP сокета
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation error");
        return 1;
    }

    // Настройка сервера
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    server_addr.sin_port = htons(port);

    // Привязка сокета к IP адресу и порту
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Binding error");
        return 1;
    }

    // Ожидание входящих подключений
    if (listen(server_fd, 5) < 0) {
        perror("Listening error");
        return 1;
    }

    printf("Server started. Waiting for connections...\n");

    int clients_finished = 0;

    while (clients_finished < rows) {
        client_len = sizeof(client_addr);

        // Принятие входящего подключения
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            perror("Accepting error");
            return 1;
        }

        printf("New connection accepted\n");

        // Получение данных от клиента
        ssize_t num_bytes = recv(client_fd, buffer, MAX_BUFFER_SIZE, 0);
        if (num_bytes <= 0) {
            perror("Receiving error");
            return 1;
        }

        // Обработка данных клиента
        int num_books = num_bytes / sizeof(struct Book);
        struct Book* received_books = (struct Book*)buffer;

        int row = received_books[0].row;

        for (int i = 0; i < num_books; i++) {
            struct Book* book = &received_books[i];
            int book_count = catalog.book_count[row];

            if (book_count < MAX_BOOKS) {
                catalog.books[row][book_count] = *book;
                catalog.book_count[row]++;
            }
        }

        printf("Catalog updated by student from row %d\n", row);

        // Закрытие соединения с клиентом
        close(client_fd);
        clients_finished++;
    }

    // Сортировка и печать окончательного каталога
    sort_catalog(&catalog, rows);

    printf("Final catalog:\n");
    for (int i = 0; i < MAX_ROWS; i++) {
        int book_count = catalog.book_count[i];
        for (int j = 0; j < book_count; j++) {
            struct Book* book = &catalog.books[i][j];
            printf("Identifier: %s, Row: %d, Shelf: %d, Book Number: %d\n", book->identifier, book->row, book->shelf, book->book_number);
        }
    }

    // Закрытие серверного сокета
    close(server_fd);

    return 0;
}
