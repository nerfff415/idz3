**Безруков Михаил Александрович БПИ 214** 
**Вариант 22**

**Задача об инвентаризации по рядам.** После нового года в библиотеке университета обнаружилась пропажа каталога. После поиска и наказания виноватых, ректор дал указание восстановить каталог силами студентов. Фонд библиотека представляет собой прямоугольное помещение, в котором находится M рядов по N шка8
фов по K книг в каждом шкафу. Требуется создать приложение, составляющее каталог. В качестве отдельного процесса
задается составление подкаталога одним студентом–процессом для
одного ряда. После этого студент передает информацию процессу–
библиотекарю, который сортирует ее учитывая подкаталоги, переданные другими студентами. Сервер формирует каталог, получая
информацию от студентов — клиентов. Порождение студентов
может быть вручную или с использованием скрипта. Примечание. Каталог — это список книг, упорядоченный по их
инвентарному номеру или по алфавиту (на выбор разработчика). Каждая строка каталога содержит идентифицирующее значение (номер или название), номер ряда,
номер шкафа, номер книги в шкафу.

**В данном сценарии у нас есть следующие сущности:**
**Сервер:** библиотекарь, который будет принимать информацию от студентов-клиентов и формировать каталог. Он будет ожидать подключений от клиентов и принимать данные через TCP сокет.
**Клиенты:** студенты, которые составляют подкаталоги для своих рядов. Каждый студент-клиент будет иметь собственный процесс и будет соединяться с сервером через TCP сокет. Они передадут свои подкаталоги серверу для дальнейшей сортировки.
Процесс взаимодействия между сервером и клиентами следующий:
Сервер запускается и ожидает подключений от клиентов. Он создает TCP сокет, связывается с определенным IP адресом и портом, и начинает слушать входящие подключения.
Каждый студент-клиент запускает свой процесс для составления подкаталога своего ряда. Он создает TCP сокет, соединяется с сервером (используя IP адрес и порт сервера) и начинает передавать информацию о книгах в своем ряду через сокет.
Сервер принимает входящее соединение от студента-клиента и получает данные о книгах в его подкаталоге. Он обрабатывает эти данные и сортирует их в общий каталог, учитывая информацию от других студентов-клиентов.
Когда все студенты-клиенты завершили передачу своих подкаталогов, сервер завершает прием данных и формирует окончательный каталог, упорядоченный по алфавиту и ряду.

gcc server.c -o server
gcc client.c -o client
./server <ip_address> <port> <rows>
(Пример: ./server 127.0.0.2 8080 2)
./client <ip_address> <port> <row>
(Пример: ./client 127.0.0.2 8080 1)

Тестирование:
_______________________________________________________
user@LAPTOP-OC5FIHNI:~/HW_OS$ ./client 127.0.0.2 8080 1
  
Enter book details (identifier, shelf, book number):
  
Book 1:
  
Identifier: ZZZ
  
Shelf: 1
  
Book Number: 1
  
Enter more books? (y/n): y
  
Book 2:
  
Identifier: aaa
  
Shelf: 5
  
Book Number: 5
  
Enter more books? (y/n): n
  
Subcatalog sent to server
_______________________________________________________
user@LAPTOP-OC5FIHNI:~/HW_OS$ ./client 127.0.0.2 8080 2
  
Enter book details (identifier, shelf, book number):
  
Book 1:
  
Identifier: qwe
  
Shelf: 1
  
Book Number: 2
  
Enter more books? (y/n): y
  
Book 2:
  
Identifier: A
  
Shelf: 1
  
Book Number: 3
  
Enter more books? (y/n): n
  
Subcatalog sent to server
_______________________________________________________
user@LAPTOP-OC5FIHNI:~/HW_OS$ ./server 127.0.0.2 8080 2
  
Server started. Waiting for connections...
  
New connection accepted
  
Catalog updated by student from row 1
  
New connection accepted
  
Catalog updated by student from row 2
  
Final catalog:
  
Identifier: ZZZ, Row: 1, Shelf: 1, Book Number: 1
  
Identifier: aaa, Row: 1, Shelf: 5, Book Number: 5
  
Identifier: qwe, Row: 2, Shelf: 1, Book Number: 2
  
Identifier: A, Row: 2, Shelf: 1, Book Number: 3
_______________________________________________________
