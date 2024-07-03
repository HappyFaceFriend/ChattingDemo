//https://navendus.tripod.com/winsock/winsock20_01.htm


#include <iostream>
#include <winsock.h>

#include <RamenNetworking.h>

#pragma comment(lib, "ws2_32.lib") // Link with Winsock library

constexpr int PORT = 8080;
constexpr int MAX_CLIENTS = 10;

constexpr char SERVER_IP_ADDRESS[] = "127.0.0.1";

struct ClientInfo
{
	SOCKET clientSocket;
	struct sockaddr_in clientAddr;
};

bool ClientThread(LPVOID lpData)
{
	ClientInfo* clientInfo = reinterpret_cast<ClientInfo*>(lpData);
	char buffer[1024];
	while (true)
	{
		int length = recv(clientInfo->clientSocket, buffer, sizeof(buffer), 0);
		if (length > 0)
		{
			buffer[length] = '\0';
			std::cout << "Recieved " << buffer << " from " << inet_ntoa(clientInfo->clientAddr.sin_addr) << '\n';

			// Send it back
			if (strcmp(buffer, "QUIT") == 0)
			{
				closesocket(clientInfo->clientSocket);
				return true;
			}

			// Split message if it is too long
			int sendCount = 0;
			char* bufferOffset = buffer;

			while ((sendCount = send(clientInfo->clientSocket, bufferOffset, length, 0)) != length)
			{
				if (sendCount == -1)
				{
					std::cerr << "Error sending data to "<< inet_ntoa(clientInfo->clientAddr.sin_addr) << '\n';
					break;
				}
				bufferOffset += sendCount;
				length -= sendCount;
			}
		}
		else
		{
			std::cerr << "Error reading data from " << inet_ntoa(clientInfo->clientAddr.sin_addr) << '\n';
			closesocket(clientInfo->clientSocket);
		}
	}
	return true;
}

int main()
{
	// Init WSAData
	auto initResult = RamenNetworking::NetworkSystem::Init();
	if (initResult == RamenNetworking::NetworkSystem::Result::Success)
		std::cout << "Init Success!\n";

	// Create socket
	SOCKET serverSocket; //uint
	serverSocket = socket(AF_INET, SOCK_STREAM, 0); // Address family, protocol type, protocol name
	if (serverSocket == INVALID_SOCKET)
	{
		std::cerr << "Socket creation failed\n";
		WSACleanup();
		return 1;
	}

	// Create structure describing server parameters
	sockaddr_in serverAddr; // IP Address
	serverAddr.sin_family = AF_INET; // Address family
	serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP_ADDRESS);
	serverAddr.sin_port = htons(PORT);

	// Bind server socket to address and port
	auto bindResult = bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr));
	if (bindResult == SOCKET_ERROR)
	{
		std::cerr << "Unable to bind to " << SERVER_IP_ADDRESS << ":" << PORT << "\n";
		closesocket(serverSocket);
		WSACleanup();
		return -1;
	}

	// Put server socket to listen state
	auto listenResult = listen(serverSocket, SOMAXCONN);
	if (listenResult == SOCKET_ERROR)
	{
		std::cerr << "Unable to put server to listen state\n";
		closesocket(serverSocket);
		WSACleanup();
		return -1;
	}

	std::cout << "Server socket is now in listening state ...\n";

	while (true)
	{
		// As the socket is in listen mode there is a connection request pending.
		// Calling accept( ) will succeed and return the socket for the request.
		SOCKET clientSocket;
		struct sockaddr_in clientAddr;
		int clientAddrSize = sizeof(clientAddr);

		clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrSize);
		if (clientSocket == INVALID_SOCKET)
		{
			std::cerr << "accept() failed\n";
		}
		else
		{
			HANDLE clientThread;
			DWORD threadID;

			struct ClientInfo clientInfo;
			clientInfo.clientAddr = clientAddr;
			clientInfo.clientSocket = clientSocket;

			std::cout << "Client connected from " << inet_ntoa(clientAddr.sin_addr) << '\n';

			// Start client thread
			clientThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ClientThread, (LPVOID)&clientInfo, 0, &threadID);
			if (clientThread == NULL)
			{
				std::cerr << "Unable to create client thread\n";
			}
			else
			{
				CloseHandle(clientThread);
			}
		}
	}
	closesocket(serverSocket);
	WSACleanup();
	return 0;

}