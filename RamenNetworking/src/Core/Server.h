#pragma once

#include <thread>
#include <queue>

#include "Defs.h"
#include "Networking/ServerSocket.h"
#include "Utilities/MessageQueue.h"

namespace RamenNetworking
{
	class Server
	{
	public:
		Server(size_t messageSize = DEFAULT_MESSAGE_SIZE, size_t messageQueueSize = DEFAULT_MESSAGE_QUEUE_SIZE);
		~Server();

		Server(const Server&) = delete;
		Server& operator=(const Server&) = delete;

		Result Init(const Address& serverAddress);
		void StartListening();
		Result SendMessageToAllClients(char* buffer, uint32_t bufferSize = DEFAULT_MESSAGE_SIZE);

		bool TryPollMessage(std::vector<char>& message);

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
		const size_t m_MessageSize;
		size_t m_MessageQueueSize; // This is not const because this might provide resizing methods in the future.
		MessageQueue<std::vector<char>> m_MessageQueue;

		ServerSocket m_Socket{};
		std::thread m_ListenThread{};
		std::thread m_RecieveThread{};
		Address m_ServerAddress{};

		// TEMP: This should be thread safe
		std::vector<ClientInfo> m_ClientInfos{};
		// TODO: This should be thread safe
		bool m_IsListening = false;
		bool m_IsRecieving = false;
	};
}