#include <stdio.h> /*чтобы писать на экран*/
#pragma comment(lib,"Ws2_32.lib")

#include <sys/types.h>
#include <WinSock2.h>
#include <Ws2tcpip.h> 
#include <locale.h>

#include <stdint.h> //для типа int32_t 

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
		int SlaveSocket = accept(MasterSocket, 0, 0); /*приняли соединение, появился новый сокет*/
		printf("accept %d",SlaveSocket);
		printf("\n");
		printf("Клиент подключился\n");

		/*создадим буфер из пяти нулей (байт)*/
		/*читаем и выводим на экран соединения которые прочитали*/
		char Buffer[1024]; /*char Buffer[1024]; char Buffer[5] = {0, 0, 0, 0, 0};*/

		//int len = recv(SlaveSocket, Buffer, sizeof(int32_t), NULL); /*принимаем известной длины 32 бита длину сообщения*/

		int res = recv(SlaveSocket, Buffer, 4, NULL);
		
		send(SlaveSocket, Buffer, 4, NULL);
		shutdown(SlaveSocket, SD_BOTH);
		closesocket(SlaveSocket);

		printf("Сообщение: ");
		printf("%s\n", Buffer);
		getchar ();
	}

	return 0;
}