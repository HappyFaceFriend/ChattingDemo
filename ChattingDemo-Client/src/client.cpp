//https://navendus.tripod.com/winsock/winsock20_01.htm

#include <iostream>

#include <RamenNetworking.h>

constexpr int SERVER_PORT = 8080;
constexpr char SERVER_IP_ADDRESS[] = "127.0.0.1";

int main()
{
	std::string serverIP = SERVER_IP_ADDRESS;
	unsigned short serverPort = SERVER_PORT;
	std::cout << "Enter server IP: ";
	std::cin >> serverIP;
	std::cout << "Enter server Port: ";
	std::cin >> serverPort;

	auto networkAPI = RamenNetworking::NetworkAPI::Create();

	if (!networkAPI->IsValid())
	{
		std::cerr << "Initailization Failed.\n";
		return 1;
	}
	
	auto clientSocket = RamenNetworking::ClientSocket::Create();

	if (!clientSocket->IsValid())
	{
		return 1;
	}

	auto result = clientSocket->Connect({ serverIP, serverPort});
	if (result == RamenNetworking::Result::Fail)
	{
		return 1;
	}

	char msgBuffer[1024] = { 0 };

	while (strcmp(msgBuffer, "QUIT") != 0)
	{
		std::cout << "Enter Message (QUIT to quit): ";
		std::cin >> msgBuffer;

		int msgLength = strlen(msgBuffer) + 1;

		// Split message if too long
		char* bufferOffset = msgBuffer;

		while (msgLength > 0)
		{
			result = clientSocket->Send(bufferOffset, msgLength);
			if (result == RamenNetworking::Result::Fail)
			{
				std::cerr << "Error sending data to server\n";
				break;
			}
			bufferOffset += sizeof(msgBuffer);
			msgLength -= sizeof(msgBuffer);
		}
		if (strcmp(msgBuffer, "QUIT") == 0)
		{
			break;
		}

		result = clientSocket->Recv(msgBuffer, sizeof(msgBuffer));
		if (result == RamenNetworking::Result::Success)
		{
			std::cout << "Recieved: " << msgBuffer << '\n';
		}
	}
	clientSocket->Close();
	return 0;
}