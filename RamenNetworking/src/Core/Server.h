#pragma once

#include <thread>
#include <queue>

#include "Networking/ServerSocket.h"

namespace RamenNetworking
{
	class Server
	{
	public:
		Server();
		~Server();

		Server(const Server&) = delete;
		Server& operator=(const Server&) = delete;

		Result Init(const Address& serverAddress);
		void StartListening();
		Result SendMessageToAllClients(char* buffer, uint32_t bufferSize);

		std::queue<std::vector<char>>& GetMessageQueue() { return m_MessageQueue; }

	private:
		struct ClientInfo
		{
			ClientSocket socket;
			Address address;
			
			ClientInfo(ClientSocket&& socket, Address&& address) : socket(std::move(socket)), address(std::move(address)){}
		};

	private:
		void ListenThreadFunc();
		void RecieveThreadFunc();

	private:
		ServerSocket m_Socket{};
		std::thread m_ListenThread{};
		std::thread m_RecieveThread{};
		Address m_ServerAddress{};

		// TEMP: This should be thread safe
		std::queue<std::vector<char>> m_MessageQueue{};
		// TEMP: This should be thread safe
		std::vector<ClientInfo> m_ClientInfos{};
		// TODO: This should be thread safe
		bool m_IsListening = false;
		bool m_IsRecieving = false;
	};
}