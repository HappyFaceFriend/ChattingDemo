//https://navendus.tripod.com/winsock/winsock20_01.htm

#include <iostream>

#include <windows.h>
#include <RamenNetworking.h>

constexpr int SERVER_PORT = 8080;
constexpr char SERVER_IP_ADDRESS[] = "127.0.0.1";
constexpr int MAX_CLIENTS = 10;

using Result = RamenNetworking::Result;

struct ClientInfo
{
	std::unique_ptr<RamenNetworking::ClientSocket> clientSocket;
	RamenNetworking::Address clientAddress;
};

bool ClientThread(LPVOID lpData)
{
	ClientInfo* clientInfo = reinterpret_cast<ClientInfo*>(lpData);
	char buffer[1024];
	while (true)
	{
		auto result = clientInfo->clientSocket->Recv(buffer, sizeof(buffer));
		if (result == Result::Success)
		{
			std::cout << "Recieved " << buffer << " from " << clientInfo->clientAddress.IPAddress << ":" << clientInfo->clientAddress.PortNumber << '\n';

			// Send it back
			if (strcmp(buffer, "QUIT") == 0)
			{
				clientInfo->clientSocket->Close();
				return true;
			}

			// Split message if it is too long
			char* bufferOffset = buffer;
			int msglength = strlen(buffer) + 1;
			while (msglength > 0)
			{
				result = clientInfo->clientSocket->Send(bufferOffset, sizeof(buffer));
				if (result == Result::Fail)
				{
					std::cerr << "Error sending data to " << clientInfo->clientAddress.IPAddress << '\n';
					break;
				}
				bufferOffset += sizeof(buffer);
				msglength -= sizeof(buffer);
			}
		}
		else
		{
			std::cerr << "Error reading data from " << clientInfo->clientAddress.IPAddress << '\n';
			clientInfo->clientSocket->Close();
		}
	}
	return true;
}

int main()
{

	// Init WSAData
	auto initResult = RamenNetworking::NetworkAPI::Init();
	if (initResult == RamenNetworking::Result::Fail)
	{
		std::cerr << "Initailization Failed.\n";
		return 1;
	}

	auto serverSocket = RamenNetworking::ServerSocket::Create();

	auto result = serverSocket->Init();
	if (result == Result::Fail)
	{
		RamenNetworking::NetworkAPI::Cleanup();
		return 1;
	}

	result = serverSocket->Bind({ SERVER_IP_ADDRESS, SERVER_PORT });
	if (result == Result::Fail)
	{
		RamenNetworking::NetworkAPI::Cleanup();
		return 1;
	}

	// Put server socket to listen state
	result = serverSocket->Listen(5);
	if (result == Result::Fail)
	{
		RamenNetworking::NetworkAPI::Cleanup();
		return 1;
	}

	std::cout << "Server socket is now in listening state ...\n";

	while (true)
	{
		auto [clientSocket, clientAddress] = serverSocket->Accept();
		if (clientSocket.get() == nullptr)
		{
			std::cerr << "accept() failed\n";
		}
		else
		{
			HANDLE clientThread;
			DWORD threadID;

			// TODO : delete this
			ClientInfo* clientInfo = new ClientInfo{ std::move(clientSocket), clientAddress };

			std::cout << "Client connected from " << clientAddress.IPAddress << '\n';

			// Start client thread
			clientThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ClientThread, (LPVOID)clientInfo, 0, &threadID);
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
	serverSocket->Close();
	RamenNetworking::NetworkAPI::Cleanup();
	return 0;

}