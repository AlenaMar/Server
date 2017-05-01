#include <stdio.h> /*чтобы писать на экран*/
#pragma comment(lib,"Ws2_32.lib")

#include <sys/types.h>
#include <WinSock2.h>
#include <Ws2tcpip.h> 
#include <locale.h>

#include <stdint.h> //для типа int32_t 
#include <ctime>
#include <iostream>
#include <string>
#include <math.h>

int ClientCount = 0; /*кол-во подключаемых пользователей, изначально 0*/

//массив структур, в котором для каждого пользователя будет храниться сокет соединения и идентификатор потока
struct ClientInfo
{
	int socket; /*сокет подключаемого пользователя*/
	int id_thread; /*идентификатор потока для клиента*/
};
struct ClientInfo clients[100]; //массив из этих структур


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

//функция для передачи длины сообщения, затем его содержимого
void SendMessageToClient (int cl)
{
	/*ПЕРЕДАЕМ ДЛИНУ СООБЩЕНИЯ*/
		
		int32_t messageLength = 0; //32-разрядное целое число из 4 байтов, длина собщения

		char messageBuffer[1000]; //Массив из 1000 chars, т.е. байтов

		//Передаем в recv адрес переменной messageLength и ее длину в памяти
		//Инструкция принять 4 байта и записать по адресу messageLength
		size_t receivedLength = recv(clients[cl].socket, (char *)&messageLength, sizeof(messageLength), NULL);

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
		receivedLength = recv(clients[cl].socket, messageBuffer, messageLength, NULL);

		// Клиент должен прислать столько, сколько обещал. Обрезанные сообщения не нужны
		if (receivedLength != messageLength)
			exit(0);

		// За концом строки надо поставить нулевой байт, если хотим ее распечатать
		messageBuffer[messageLength] = 0;
		printf("      Полученная строка: '%s'\n", messageBuffer);

		// Теперь передаем ее обратно

		for (int i = 0; i < ClientCount; i++)
		{
			if (exit(messageBuffer))
			{
				out_m(messageBuffer);
				strcat_s(messageBuffer, " покинул(а) чат");
				for (int i = 0; i <= ClientCount; i++)
				{
					if (i == clients[cl].socket)continue;
					send(clients[cl].socket, messageBuffer, 512, NULL);
				}
				closesocket(clients[cl].socket);
				decompres(clients[cl].socket);
				ClientCount--;
				break;
			}
			
			// Передаем счетчик длины фиксированного размера - sizeof(messageLength), т.е. 4 байта
			send(clients[i].socket, (char *)&messageLength, sizeof(messageLength), NULL); 

			// Передаем строку, ровно столько байт, сколько приняли
			send(clients[i].socket, messageBuffer, messageLength, NULL);

			shutdown(clients[i].socket, SD_BOTH);
			closesocket(clients[i].socket);
		}
		
}

//Функция для выхода
bool exit(char * out)
{
	int i = 0;
	char ex[] = "EXIT";
	char*temp = out;
	while (temp[0] != ':')
		temp++;
	temp++;
	temp++;
	if (strcmp(temp, ex) == 0)
		return true;
	return false;
}

void out_m(char * msg)
{
	char name[500];
	char * temp = msg;
	int i = 0;
	while (temp[0] != ':')
	{
		name[i++] = temp[0];
		temp++;
	}
	ZeroMemory(msg, sizeof(char) * 512);
	for (int j = 0; j < i; j++)
		msg[j] = name[j];
}

//Функция для отключения
void decompres(int n)
{
	for (int i = n; i < ClientCount - 1; i++)
	{
		clients[i].socket = clients[i + 1].socket;
		clients[i].socket = clients[i].socket - 1;
	}
}

//выясняет, отлаживается ли вызывающий процесс (динамическая защита)
void fastcall (int p)
{
	bool check1 = IsDebuggerPresent();
	if (check1) {
		printf("Меня отлаживают...\n");
		exit(0);
	}
	else
		printf("Нет отладчика...\n");
}

int main(int args, char **argv)
{
	//для защиты ключом просим ввести ключ
	setlocale(LC_ALL, "Russian");
	float t1 = 0.568207;
	char Key[20];
	printf("Введите ключ: ");
	gets(Key);

	int pr = int(Key[0]);
	for (int i = 1; i < strlen(Key); i++)
	{
		pr = pr*int(Key[i]);
	}

	float t = sin(pr);

	if (abs(t1 - t) < 0.00001)
	{
		printf("***Верный ключ***\n\n");
	}
	else
	{
		printf("Неверный ключ\n");
		exit(0);
	}

	printf("Добро пожаловать в мастер создания сервера!\n");
	WSAData ws;
	WSAStartup(MAKEWORD(2, 2), &ws);
	
	int MasterSocket = socket(AF_INET/*IPv4*/, SOCK_STREAM /*TCP*/, IPPROTO_TCP); /*создали сокет*/

	struct sockaddr_in SockAddr;
	struct sockaddr_in * result;
	ZeroMemory(&SockAddr, sizeof(SockAddr));
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_port = htonl(12345);
	SockAddr.sin_addr.s_addr = INADDR_ANY; /*0.0.0.0*/

	char iport[] = "";
	char port[] = "";
	//Установка ip и порта
	printf("Введите ip-адрес: ");
	gets(iport);
	printf("Введите port : ");
	gets(port);

	fastcall(bind(MasterSocket, (struct sockaddr *)(&SockAddr), sizeof(SockAddr)));

	fastcall(listen(MasterSocket, SOMAXCONN)); /*слушаем сокет*/
	printf("Ждем подключений...\n");

	/*входим в бесконечный цикл*/
	while (1)
	{
		if (int SlaveSocket = accept(MasterSocket, 0, 0)){ /*приняли соединение, появился новый сокет*/
						printf("   *Клиент №: %d подключился \n", ClientCount);

			printf("      Сокет соединения: %d \n", SlaveSocket);
			clients[ClientCount].socket = SlaveSocket;
			clients[ClientCount].socket = SlaveSocket; /*сохраняем сокет подключаемого клиента*/
			ClientCount++; /*увеличиваем на 1*/

			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)SendMessageToClient, (LPVOID)(ClientCount-1), 0, 0); //поток
			clients[ClientCount].id_thread = GetCurrentThreadId(); //присваиваем идентификатор потока
			printf("      Идентификатор потока: %d\n", clients[ClientCount].id_thread);
		}
	}

	getchar ();
	return 0;
}