#include <stdio.h> /*����� ������ �� �����*/
#pragma comment(lib,"Ws2_32.lib")

#include <sys/types.h>
#include <WinSock2.h>
#include <Ws2tcpip.h> 
#include <locale.h>

#include <stdint.h> //��� ���� int32_t 

int Users[10]; /*v3 ��������� �������, ������ � ������� ������������ ������������*/
int ClientCount = 0; /*v3 ���-�� ������������ �������������, ���������� 0*/

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


void SendMessageToClient (int cl)
{
	/*�������� ����� ���������*/
		
		int32_t messageLength = 0; //32-��������� ����� ����� �� 4 ������, ����� ��������

		char messageBuffer[1000]; //������ �� 1000 chars, �.�. ������

		//�������� � recv ����� ���������� messageLength � �� ����� � ������
		//���������� ������� 4 ����� � �������� �� ������ messageLength
		size_t receivedLength = recv(Users[cl], (char *)&messageLength, sizeof(messageLength), NULL);

		//��������� ����� ���������. ������ ������ �������� sizeof(messageLength) ����.
		if (receivedLength != sizeof(messageLength))
		{
			//������ ������ � �����������
			exit(1);
		}

		/*��������� � ����� ���� ���������*/

		//���������, ����� ���� ��������� �� ������� �� ������� ������
		if (messageLength > sizeof(messageBuffer) - 1)
		{
			// �� ���� ������ ������ ������, � ������, ��������������� ��� ������,
			// ������ ����������� ������� ������. ���� �������� ����� ��� ����, ������� ��� ����� 1.
			messageLength = sizeof(messageBuffer) - 1;
		}

		//�������� �����
		receivedLength = recv(Users[cl], messageBuffer, messageLength, NULL);

		// ������ ������ �������� �������, ������� ������. ���������� ��������� �� �����
		if (receivedLength != messageLength)
			exit(0);

		// �� ������ ������ ���� ��������� ������� ����, ���� ����� �� �����������
		messageBuffer[messageLength] = 0;
		printf("���������� ������: '%s'\n", messageBuffer);

		// ������ �������� �� �������

		// �������� ������� ����� �������������� ������� - sizeof(messageLength), �.�. 4 �����
		send(Users[cl], (char *)&messageLength, sizeof(messageLength), NULL); 

		// �������� ������, ����� ������� ����, ������� �������
		send(Users[cl], messageBuffer, messageLength, NULL);

		shutdown(Users[cl], SD_BOTH);
		closesocket(Users[cl]);
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
		if (int SlaveSocket = accept(MasterSocket, 0, 0)){ /*������� ����������, �������� ����� �����*/
			printf("������ �����������\n");
			printf("*e�� �����: %d \n", SlaveSocket); //v3
			printf("*������ � : %d \n", ClientCount); //v3

			Users[ClientCount] = SlaveSocket; //v3/*��������� ����� ������������� �������*/
			ClientCount++; /*����������� �� 1*/

			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)SendMessageToClient, (LPVOID)(ClientCount-1), 0, 0); //�����
		}
	}
	getchar ();
	return 0;
}