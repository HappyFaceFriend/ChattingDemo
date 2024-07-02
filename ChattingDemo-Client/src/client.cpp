//https://navendus.tripod.com/winsock/winsock20_01.htm

#include <iostream>
#include <winsock.h>

#include <Test.h>

#pragma comment(lib, "ws2_32.lib") // Link with Winsock library

constexpr int SERVER_PORT = 8080;
constexpr char SERVER_IP_ADDRESS[] = "127.0.0.1";

int main()
{
	//int port = 8080;
	//std::cout << "Enter Port: ";
	//std::cin >> port;
	
	Test t;
	t.F();

	// Init WSAData
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		std::cerr << "WSAStartup failed\n";
		return 1;
	}

	// Create socket
	SOCKET clientSocket; //uint
	clientSocket = socket(AF_INET, SOCK_STREAM, 0); // Address family, protocol type, protocol name
	if (clientSocket == INVALID_SOCKET)
	{
		std::cerr << "Socket creation failed\n";
		WSACleanup();
		return 1;
	}

	// Create structure describing server parameters
	sockaddr_in serverAddr; // IP Address
	serverAddr.sin_family = AF_INET; // Address family
	serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP_ADDRESS);
	serverAddr.sin_port = htons(SERVER_PORT);

	// Connect to server
	auto result = connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr));
	if (result < 0)
	{
		std::cerr << "Unable to connect to " << SERVER_IP_ADDRESS << ":" << SERVER_PORT << "\n";
		closesocket(clientSocket);
		WSACleanup();
		return -1;
	}

	char msgBuffer[1024] = "";

	while (strcmp(msgBuffer, "QUIT") != 0)
	{
		std::cout << "Enter Message (QUIT to quit): ";
		std::cin >> msgBuffer;

		int msgLength = strlen(msgBuffer);

		// Split message if too long
		int sendCount = 0;
		char* bufferOffset = msgBuffer;

		while ((sendCount = send(clientSocket, bufferOffset, msgLength, 0)) != msgLength)
		{
			if (sendCount == -1)
			{
				std::cerr << "Error sending data to server\n";
				break;
			}
			bufferOffset += sendCount;
			msgLength -= sendCount;
		}
		if (strcmp(msgBuffer, "QUIT") == 0)
		{
			break;
		}

		msgLength = recv(clientSocket, msgBuffer, sizeof(msgBuffer), 0);
		if (msgLength > 0)
		{
			msgBuffer[msgLength] = '\0';
			std::cout << "Recieved: " << msgBuffer << '\n';
		}
	}
	closesocket(clientSocket);
	WSACleanup();
	return 0;
}