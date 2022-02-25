#include <Winsock.h>
#include <iostream>
#include <string>

using namespace std;

#pragma comment(lib, "ws2_32.lib")

struct NumberPacket
{
	int Number1;
	int Number2;
};

int main()
{
	WSAData wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		cout << "Winsock Error : " << GetLastError() << endl;
		exit(-1);
	}

	SOCKET ServerSocket;

	ServerSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (ServerSocket == INVALID_SOCKET)
	{
		cout << "socket Error :" << GetLastError() << endl;
		exit(-1);
	}

	SOCKADDR_IN ServerAddr;

	memset(&ServerAddr, 0, sizeof(ServerAddr));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(50000);
	ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(ServerSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)
	{
		cout << "bind Error : " << GetLastError() << endl;
		exit(-1);
	}

	if (listen(ServerSocket, 0) == SOCKET_ERROR)
	{
		cout << "listen Error : " << GetLastError() << endl;
		exit(-1);
	}

	while (1)
	{

		SOCKADDR_IN ClientAddr;
		int ClinetAddrSize = sizeof(ClientAddr);
		SOCKET ClientSocket;
		ClientSocket = accept(ServerSocket, (SOCKADDR*)&ClientAddr, &ClinetAddrSize);
		if (ClientSocket == SOCKET_ERROR)
		{
			cout << "accept Error : " << GetLastError() << endl;
			exit(-1);
		}

		cout << "connect ip : " << inet_ntoa(ClientAddr.sin_addr) << endl;
		cout << "connect port : " << ntohs(ClientAddr.sin_port) << endl;

		NumberPacket Packet;
		recv(ClientSocket, (char*)(&Packet), 8, 0);

		cout << "client sended : " << Packet.Number1 << endl;
		cout << "client sended : " << Packet.Number1 << endl;

		int IntResult = Packet.Number1 + Packet.Number2;

		string Result = to_string(IntResult);

		send(ClientSocket, Result.c_str(), Result.length() + 1, 0);

		cout << "client send : " << Result << endl;

		closesocket(ClientSocket);
	}

	closesocket(ServerSocket);

	WSACleanup();

	return 0;
}