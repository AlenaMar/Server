#include <stdio.h> /*����� ������ �� �����*/
#pragma comment(lib,"Ws2_32.lib")

#include <sys/types.h>
#include <WinSock2.h>
#include <Ws2tcpip.h> 
#include <locale.h>

#include <stdint.h> //��� ���� int32_t 

//������� ��� �������� �� ������
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
	
	int MasterSocket = socket(AF_INET/*IPv4*/, SOCK_STREAM /*TCP*/, IPPROTO_TCP); /*������� �����*/

	struct sockaddr_in SockAddr;
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_port = htons(12345);
	SockAddr.sin_addr.s_addr = INADDR_ANY; /*0.0.0.0*/

	CheckForError(bind(MasterSocket, (struct sockaddr *)(&SockAddr), sizeof(SockAddr)));
	
	//DWORD er = WSAGetLastError();
	//printf("%d",er);
	//printf("\n");

	CheckForError(listen(MasterSocket, SOMAXCONN)); /*������� �����*/
	printf("������������� �������� ���������...\n");

	/*������ � ����������� ����*/
	while (1)
	{
		int SlaveSocket = accept(MasterSocket, 0, 0); /*������� ����������, �������� ����� �����*/
		printf("accept %d",SlaveSocket);
		printf("\n");
		printf("������ �����������\n");

		/*�������� ����� �� ���� ����� (����)*/
		/*������ � ������� �� ����� ���������� ������� ���������*/
		char Buffer[1024]; /*char Buffer[1024]; char Buffer[5] = {0, 0, 0, 0, 0};*/

		//int len = recv(SlaveSocket, Buffer, sizeof(int32_t), NULL); /*��������� ��������� ����� 32 ���� ����� ���������*/

		int res = recv(SlaveSocket, Buffer, 4, NULL);
		
		send(SlaveSocket, Buffer, 4, NULL);
		shutdown(SlaveSocket, SD_BOTH);
		closesocket(SlaveSocket);

		printf("���������: ");
		printf("%s\n", Buffer);
		getchar ();
	}

	return 0;
}