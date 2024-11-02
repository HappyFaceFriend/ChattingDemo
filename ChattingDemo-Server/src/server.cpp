//https://navendus.tripod.com/winsock/winsock20_01.htm

#include <iostream>

#include <RamenNetworking.h>

constexpr int SERVER_PORT = 8080;
constexpr char SERVER_IP_ADDRESS[] = "127.0.0.1";
constexpr int MAX_CLIENTS = 10;

using Result = RamenNetworking::Result;

int main()
{
	std::cout << "CHATTING DEMO - SERVER\n";

	RamenNetworking::TCPServer server;
	auto result = server.Init({ SERVER_IP_ADDRESS, SERVER_PORT });
	if (result == Result::Fail)
	{
		std::cout << "Failed initializing server\n";
		return 1;
	}

	server.StartListening([](RamenNetworking::Address& clientAddress, RamenNetworking::TCPServer::ClientID clientID) {
		std::cout << "Client " << clientID << " connected - " << clientAddress.IPAddress << ":" << clientAddress.PortNumber << std::endl;
		},
		[](RamenNetworking::Address& clientAddress, RamenNetworking::TCPServer::ClientID clientID, bool graceful) {
			std::cout << "Client " << clientID << " disconnected " << (graceful ? "gracefully" : "disgracefully")<< " - " << clientAddress.IPAddress << ":" << clientAddress.PortNumber << std::endl;
		}
	);

	while (true)
	{
		RamenNetworking::TCPServer::Message message;
		auto hasMessage = server.TryPollMessage(message);

		if (hasMessage)
		{
			std::cout << "Received from client" << message.id << ": " << message.message << "\n";
			if (strcmp(message.message.c_str(), "QUIT") == 0)
			{
				server.DisconnectClient(message.id, true);
				std::cout << "Disconnected " << message.id << "\n";
				continue;
			}
			else
			{
				std::cout << "Broadcasting: " << message.message << "\n";
				server.SendMessageToAllClients(message.message.c_str());
			}
		}
	}
	return 0;

}