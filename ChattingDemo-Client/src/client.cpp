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

	RamenNetworking::Client client;
	client.Init();

	client.ConnectToServer({ serverIP, serverPort });

	char msgBuffer[1024] = { 0 };

	while (strcmp(msgBuffer, "QUIT") != 0)
	{
		std::cout << "Enter Message (QUIT to quit): ";
		std::cin >> msgBuffer;

		client.SendMessageToServer(msgBuffer, sizeof(msgBuffer));
		
		if (strcmp(msgBuffer, "QUIT") == 0)
		{
			break;
		}
		
		auto& messageQueue = client.GetMessageQueue();
		while (!messageQueue.empty())
		{
			auto message = messageQueue.front();
			messageQueue.pop();

			std::cout << "Received: " << std::string(message.data()) << "\n";
		}
	}
	return 0;
}