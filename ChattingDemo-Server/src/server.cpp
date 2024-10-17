//https://navendus.tripod.com/winsock/winsock20_01.htm

#include <iostream>

#include <windows.h>
#include <RamenNetworking.h>

constexpr int SERVER_PORT = 8080;
constexpr char SERVER_IP_ADDRESS[] = "127.0.0.1";
constexpr int MAX_CLIENTS = 10;

using Result = RamenNetworking::Result;

int main()
{
	std::cout << "CHATTING DEMO - SERVER\n";

	RamenNetworking::Server server;
	auto result = server.Init({ SERVER_IP_ADDRESS, SERVER_PORT });
	if (result == Result::Fail)
	{
		std::cout << "Failed initializing server\n";
		return 1;
	}

	server.StartListening();

	while (true)
	{
		std::vector<char> message;
		auto hasMessage = server.TryPollMessage(message);

		if (hasMessage)
		{
			std::cout << "Received: " << std::string(message.data()) << "\n";
			server.SendMessageToAllClients(message.data());
		}
	}
	return 0;

}