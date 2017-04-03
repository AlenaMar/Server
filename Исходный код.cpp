#include <stdio.h> /*чтобы писать на экран*/
#pragma comment(lib,"Ws2_32.lib")

#include <sys/types.h>
#include <WinSock2.h>
#include <Ws2tcpip.h> 
#include <locale.h>

#include <stdint.h> //для типа int32_t 

int Users[10]; /*v3 коллекция сокетов, массив в котором подключенные пользователи*/
int ClientCount = 0; /*v3 кол-во подключаемых пользователей, изначально 0*/

//функция для проверки на ошибки
void CheckForError (int result)
{
    if (result != 0)
	{
		printf("FALL!!! CheckForError result = %d", result);
		printf("\n");
		exit(0);
	}
}


void SendMessageToClient (int cl)
{
	/*ПЕРЕДАЕМ ДЛИНУ СООБЩЕНИЯ*/
		
		int32_t messageLength = 0; //32-разрядное целое число из 4 байтов, длина собщения

		char messageBuffer[1000]; //Массив из 1000 chars, т.е. байтов

		//Передаем в recv адрес переменной messageLength и ее длину в памяти
		//Инструкция принять 4 байта и записать по адресу messageLength
		size_t receivedLength = recv(Users[cl], (char *)&messageLength, sizeof(messageLength), NULL);

		//Проверяем длину сообщения. Клиент обязан прислать sizeof(messageLength) байт.
		if (receivedLength != sizeof(messageLength))
		{
			//выдаем ошибку и закрываемся
			exit(1);
		}

		/*ПРИНИМАЕМ В БУФЕР САМО СООБЩЕНИЕ*/

		//Проверяем, чтобы наше сообщение не вылезло за границу буфера
		if (messageLength > sizeof(messageBuffer) - 1)
		{
			// По сети должна прийти строка, а строки, предназначенные для печати,
			// должны завершаться нулевым байтом. Надо оставить место для него, поэтому тут минус 1.
			messageLength = sizeof(messageBuffer) - 1;
		}

		//получена длина
		receivedLength = recv(Users[cl], messageBuffer, messageLength, NULL);

		// Клиент должен прислать столько, сколько обещал. Обрезанные сообщения не нужны
		if (receivedLength != messageLength)
			exit(0);

		// За концом строки надо поставить нулевой байт, если хотим ее распечатать
		messageBuffer[messageLength] = 0;
		printf("Полученная строка: '%s'\n", messageBuffer);

		// Теперь передаем ее обратно

		// Передаем счетчик длины фиксированного размера - sizeof(messageLength), т.е. 4 байта
		send(Users[cl], (char *)&messageLength, sizeof(messageLength), NULL); 

		// Передаем строку, ровно столько байт, сколько приняли
		send(Users[cl], messageBuffer, messageLength, NULL);

		shutdown(Users[cl], SD_BOTH);
		closesocket(Users[cl]);
}


int main(int args, char **argv)
{
	setlocale(LC_ALL, "russian");

	WSAData ws;
	WSAStartup(MAKEWORD(2, 2), &ws);
	
	int MasterSocket = socket(AF_INET/*IPv4*/, SOCK_STREAM /*TCP*/, IPPROTO_TCP); /*создали сокет*/

	struct sockaddr_in SockAddr;
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_port = htons(12345);
	SockAddr.sin_addr.s_addr = INADDR_ANY; /*0.0.0.0*/

	CheckForError(bind(MasterSocket, (struct sockaddr *)(&SockAddr), sizeof(SockAddr)));
	
	//DWORD er = WSAGetLastError();
	//printf("%d",er);
	//printf("\n");

	CheckForError(listen(MasterSocket, SOMAXCONN)); /*слушаем сокет*/
	printf("Прослушивание входящих сообщений...\n");

	/*входим в бесконечный цикл*/
	while (1)
	{
		if (int SlaveSocket = accept(MasterSocket, 0, 0)){ /*приняли соединение, появился новый сокет*/
			printf("Клиент подключился\n");
			printf("*eго сокет: %d \n", SlaveSocket); //v3
			printf("*клиент № : %d \n", ClientCount); //v3

			Users[ClientCount] = SlaveSocket; //v3/*сохраняем сокет подключаемого клиента*/
			ClientCount++; /*увеличиваем на 1*/

			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)SendMessageToClient, (LPVOID)(ClientCount-1), 0, 0); //поток
		}
	}
	getchar ();
	return 0;
}