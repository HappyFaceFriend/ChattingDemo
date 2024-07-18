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
	RamenNetworking::Server server;
	auto result = server.Init({ SERVER_IP_ADDRESS, SERVER_PORT });
	if (result == Result::Fail)
	{
		return 1;
	}

	server.StartListening();

	while (true)
	{
		auto& messageQueue = server.GetMessageQueue();
		while (!messageQueue.empty())
		{
			auto message = messageQueue.front();
			messageQueue.pop();

			std::cout << "Received: " << std::string(message.data()) << "\n";

			server.SendMessageToAllClients(message.data(), 1024);
		}
	}
	return 0;

}