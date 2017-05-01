#include <stdio.h> /*����� ������ �� �����*/
#pragma comment(lib,"Ws2_32.lib")

#include <sys/types.h>
#include <WinSock2.h>
#include <Ws2tcpip.h> 
#include <locale.h>

#include <stdint.h> //��� ���� int32_t 
#include <ctime>
#include <iostream>
#include <string>
#include <math.h>

int ClientCount = 0; /*���-�� ������������ �������������, ���������� 0*/

//������ ��������, � ������� ��� ������� ������������ ����� ��������� ����� ���������� � ������������� ������
struct ClientInfo
{
	int socket; /*����� ������������� ������������*/
	int id_thread; /*������������� ������ ��� �������*/
};
struct ClientInfo clients[100]; //������ �� ���� ��������


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

//������� ��� �������� ����� ���������, ����� ��� �����������
void SendMessageToClient (int cl)
{
	/*�������� ����� ���������*/
		
		int32_t messageLength = 0; //32-��������� ����� ����� �� 4 ������, ����� ��������

		char messageBuffer[1000]; //������ �� 1000 chars, �.�. ������

		//�������� � recv ����� ���������� messageLength � �� ����� � ������
		//���������� ������� 4 ����� � �������� �� ������ messageLength
		size_t receivedLength = recv(clients[cl].socket, (char *)&messageLength, sizeof(messageLength), NULL);

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
		receivedLength = recv(clients[cl].socket, messageBuffer, messageLength, NULL);

		// ������ ������ �������� �������, ������� ������. ���������� ��������� �� �����
		if (receivedLength != messageLength)
			exit(0);

		// �� ������ ������ ���� ��������� ������� ����, ���� ����� �� �����������
		messageBuffer[messageLength] = 0;
		printf("      ���������� ������: '%s'\n", messageBuffer);

		// ������ �������� �� �������

		for (int i = 0; i < ClientCount; i++)
		{
			if (exit(messageBuffer))
			{
				out_m(messageBuffer);
				strcat_s(messageBuffer, " �������(�) ���");
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
			
			// �������� ������� ����� �������������� ������� - sizeof(messageLength), �.�. 4 �����
			send(clients[i].socket, (char *)&messageLength, sizeof(messageLength), NULL); 

			// �������� ������, ����� ������� ����, ������� �������
			send(clients[i].socket, messageBuffer, messageLength, NULL);

			shutdown(clients[i].socket, SD_BOTH);
			closesocket(clients[i].socket);
		}
		
}

//������� ��� ������
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

//������� ��� ����������
void decompres(int n)
{
	for (int i = n; i < ClientCount - 1; i++)
	{
		clients[i].socket = clients[i + 1].socket;
		clients[i].socket = clients[i].socket - 1;
	}
}

//��������, ������������ �� ���������� ������� (������������ ������)
void fastcall (int p)
{
	bool check1 = IsDebuggerPresent();
	if (check1) {
		printf("���� ����������...\n");
		exit(0);
	}
	else
		printf("��� ���������...\n");
}

int main(int args, char **argv)
{
	//��� ������ ������ ������ ������ ����
	setlocale(LC_ALL, "Russian");
	float t1 = 0.568207;
	char Key[20];
	printf("������� ����: ");
	gets(Key);

	int pr = int(Key[0]);
	for (int i = 1; i < strlen(Key); i++)
	{
		pr = pr*int(Key[i]);
	}

	float t = sin(pr);

	if (abs(t1 - t) < 0.00001)
	{
		printf("***������ ����***\n\n");
	}
	else
	{
		printf("�������� ����\n");
		exit(0);
	}

	printf("����� ���������� � ������ �������� �������!\n");
	WSAData ws;
	WSAStartup(MAKEWORD(2, 2), &ws);
	
	int MasterSocket = socket(AF_INET/*IPv4*/, SOCK_STREAM /*TCP*/, IPPROTO_TCP); /*������� �����*/

	struct sockaddr_in SockAddr;
	struct sockaddr_in * result;
	ZeroMemory(&SockAddr, sizeof(SockAddr));
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_port = htonl(12345);
	SockAddr.sin_addr.s_addr = INADDR_ANY; /*0.0.0.0*/

	char iport[] = "";
	char port[] = "";
	//��������� ip � �����
	printf("������� ip-�����: ");
	gets(iport);
	printf("������� port : ");
	gets(port);

	fastcall(bind(MasterSocket, (struct sockaddr *)(&SockAddr), sizeof(SockAddr)));

	fastcall(listen(MasterSocket, SOMAXCONN)); /*������� �����*/
	printf("���� �����������...\n");

	/*������ � ����������� ����*/
	while (1)
	{
		if (int SlaveSocket = accept(MasterSocket, 0, 0)){ /*������� ����������, �������� ����� �����*/
						printf("   *������ �: %d ����������� \n", ClientCount);

			printf("      ����� ����������: %d \n", SlaveSocket);
			clients[ClientCount].socket = SlaveSocket;
			clients[ClientCount].socket = SlaveSocket; /*��������� ����� ������������� �������*/
			ClientCount++; /*����������� �� 1*/

			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)SendMessageToClient, (LPVOID)(ClientCount-1), 0, 0); //�����
			clients[ClientCount].id_thread = GetCurrentThreadId(); //����������� ������������� ������
			printf("      ������������� ������: %d\n", clients[ClientCount].id_thread);
		}
	}

	getchar ();
	return 0;
}