//https://navendus.tripod.com/winsock/winsock20_01.htm

#include <iostream>

#include <RamenNetworking.h>

constexpr int SERVER_PORT = 8080;
constexpr char SERVER_IP_ADDRESS[] = "127.0.0.1";

int main()
{
	// Init WSAData
	auto initResult = RamenNetworking::NetworkAPI::Init();
	if (initResult == RamenNetworking::Result::Fail)
	{
		std::cerr << "Initailization Failed.\n";
		return 1;
	}
	
	auto clientSocket = RamenNetworking::ClientSocket::Create();

	auto result = clientSocket->Init();
	if (result == RamenNetworking::Result::Fail)
	{
		RamenNetworking::NetworkAPI::Cleanup();
		return 1;
	}

	result = clientSocket->Connect({ SERVER_IP_ADDRESS, SERVER_PORT });
	if (result == RamenNetworking::Result::Fail)
	{
		RamenNetworking::NetworkAPI::Cleanup();
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
	RamenNetworking::NetworkAPI::Cleanup();
	return 0;
}