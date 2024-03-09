#include <iostream>
// подключение библиотеки WinApi сокетов
#include <Winsock2.h>
// отключение ошибки 4996
#pragma warning(disable: 4996)


int main()
{
	// WSADATA - это структура данных, используемая в Winsock (Windows Sockets API) 
	// для инициализации библиотеки сокетов. Она содержит информацию 
	// о версии Winsock и инициализации
	WSADATA ws;

	// инициализация использования сокетов
	WSAStartup(MAKEWORD(2, 2), &ws);

	SOCKET s;
	s = socket(AF_INET, SOCK_STREAM, 0);

	// структура с настройками соединения
	SOCKADDR_IN sa;
	memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(1234);

	// выбор клиент или сервер
	char c;
	printf("s - server\nk - client\n");
	scanf_s("%c", &c);

	//КЛИЕНТ
	if (c == 'k')
	{
		sa.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

		connect(s, (const sockaddr*)&sa, sizeof(sa));

		// фун-ия send передает данные по сети через указанный сокет
		int mas[] = { 0,8,0,3,2024 };
		send(s, (const char*)mas, sizeof(mas), 0);

		// клиент ждет ответ от сервера
		char st[20];
		memset(st, 0, sizeof(st));
		recv(s, st, sizeof(st), 0);
		printf(st);

		Sleep(4000);
	}

	// СЕРВЕР
	if (c == 's')
	{
		bind(s, (const sockaddr*)&sa, sizeof(sa));

		// фун-ия listen активирует режим прослушки соединения 
		// на наличие входящих сообщений
		// 2-й параметр - размер очереди
		listen(s, 100);

		int buf[5]; // буфер для приема сообщений от клиента
		memset(buf, 0, sizeof(buf));

		SOCKET client_socket;
		SOCKADDR_IN client_addr;
		int client_addr_size = sizeof(client_addr);

		// фун-ия accept ждет входящего соединения и выполняется
		// только, если кто-то подключился
		// accept возвращает идентификатор сокета клиента
		while (client_socket = accept(s, (sockaddr*)&client_addr, &client_addr_size))
		{
			printf("connect OK\n");

			// фун-ия recv получает данные по указанному сокету
			// recv возвращает количество прочитанных байт
			while (recv(client_socket, (char*)buf, sizeof(buf), 0) > 0)
			{
				for (int i = 0; i < 5; i++)
					std::cout << buf[i] << std::endl;

				// посылаем ответ клиенту
				const char reply[20] = "My first reply\0";
				send(client_socket, reply, sizeof(reply), 0);
			}	
		}
	}
	// закрытие сокета
	closesocket(s);


	return 0;
}


