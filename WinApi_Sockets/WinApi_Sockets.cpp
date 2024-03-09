#include <iostream>
// подключение библиотеки WinApi сокетов
#include <Winsock2.h>
// отключение ошибки 4996
#pragma warning(disable: 4996)

// функция для отправки сообщений без счетчика
void chat(SOCKET s)
{
	char message[200];
	// фун-ия WSAEventSelect - для чтения сообщения из recv без таймаута,
	// т.е. если фун-ия recv что-то прочитала, то она сразу возвращает
	// количество прочитанных байт
	WSAEventSelect(s, 0, FD_READ);

	do
	{
		// печатать сообщение можно при нажатии кнопки F1
		if (GetKeyState(VK_F1) < 0)
		{
			printf("message: ");
			scanf("\n%200[0-9a-zA-Z.,! ]", message);
			// фун-ия send передает данные по сети через указанный сокет
			send(s, (const char*)message, sizeof(message), 0);
		}

		// фун-ия recv получает данные по указанному сокету
	   // recv возвращает количество прочитанных байт
		if (recv(s, message, sizeof(message), 0) > 0)
			std::cout << message << std::endl;

	} while (GetKeyState(VK_ESCAPE) >= 0); // выход с окна клиента черз кнопку ESCAPE
}


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

		chat(s);
	}

	// СЕРВЕР
	if (c == 's')
	{
		bind(s, (const sockaddr*)&sa, sizeof(sa));

		// фун-ия listen активирует режим прослушки соединения 
		// на наличие входящих сообщений
		// 2-й параметр - размер очереди
		listen(s, 100);

		char buf[200]; // буфер для приема сообщений от клиента
		memset(buf, 0, sizeof(buf));

		SOCKET client_socket;
		SOCKADDR_IN client_addr;
		int client_addr_size = sizeof(client_addr);

		// фун-ия accept ждет входящего соединения и выполняется только, если кто-то подключился
		// accept возвращает идентификатор сокета клиента
		while (client_socket = accept(s, (sockaddr*)&client_addr, &client_addr_size))
		{
			printf("connect OK\n");

			chat(client_socket);	
		}
	}
	// закрытие сокета
	closesocket(s);


	return 0;
}


