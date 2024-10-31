//https://navendus.tripod.com/winsock/winsock20_01.htm

#include <iostream>

#include <RamenNetworking.h>
#include <Ramensoup.h>
constexpr int SERVER_PORT = 8080;
constexpr char SERVER_IP_ADDRESS[] = "127.0.0.1";

int main()
{
	glm::vec3 temp(0, 0, 0);
	std::string serverIP = SERVER_IP_ADDRESS;
	unsigned short serverPort = SERVER_PORT;
#if 0
	std::cout << "CHATTING DEMO - CLIENT\n";
	std::cout << "Enter server IP: ";
	std::cin >> serverIP;
	std::cout << "Enter server Port: ";
	std::cin >> serverPort;
#endif
	RamenNetworking::Client client;
	client.Init();

	client.ConnectToServer({ serverIP, serverPort });

	char msgBuffer[1024] = { 0 };

	while (strcmp(msgBuffer, "QUIT") != 0)
	{
		std::cout << "Enter Message (QUIT to quit): ";
		std::cin >> msgBuffer;

		client.SendMessageToServer(msgBuffer, sizeof(msgBuffer+1));
		
		if (strcmp(msgBuffer, "QUIT") == 0)
		{
			client.Disconnect();
			break;
		}
		while (true)
		{
			std::vector<char> message;
			if (!client.TryPollMessage(message))
				break;
			std::cout << "Received: " << std::string(message.data()) << "\n";
		}
	}
	return 0;
}