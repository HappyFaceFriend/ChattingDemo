#pragma once

#include <thread>
#include <queue>
#include <shared_mutex>
#include <functional>

#include "Defs.h"
#include "Networking/ServerSocket.h"
#include "Utilities/MessageQueue.h"

namespace RamenNetworking
{
	class Server
	{
	public:
		using ClientAcceptedCallback = std::function<void(Address)>;
	public:
		Server(size_t messageSize = DEFAULT_MESSAGE_SIZE, size_t messageQueueSize = DEFAULT_MESSAGE_QUEUE_SIZE);
		~Server();

		Server(const Server&) = delete;
		Server& operator=(const Server&) = delete;

		Result Init(const Address& serverAddress);
		void StartListening(const ClientAcceptedCallback& clientAcceptedCallback);
		Result SendMessageToAllClients(char* buffer, uint32_t bufferSize = DEFAULT_MESSAGE_SIZE);

		bool TryPollMessage(std::vector<char>& message);

	private:
		struct ClientInfo
		{
			ClientSocket socket;
			Address address;
			
			ClientInfo(ClientSocket&& socket, Address&& address) : socket(std::move(socket)), address(std::move(address)) {}
		};

	private:
		void ListenThreadFunc(const ClientAcceptedCallback& clientAcceptedCallback);
		void RecieveThreadFunc();

	private:
		const size_t m_MessageSize;
		size_t m_MessageQueueSize; // This is not const because this might provide resizing methods in the future.
		MessageQueue<std::vector<char>> m_MessageQueue;

		ServerSocket m_Socket{};
		std::thread m_ListenThread{};
		std::thread m_RecieveThread{};
		Address m_ServerAddress{};

		ClientAcceptedCallback m_ClientAcceptedCallback;

		// TEMP: This should be thread safe
		std::vector<ClientInfo> m_ClientInfos{};
		std::shared_mutex m_ClientInfosLock;
		// TODO: This should be thread safe
		bool m_IsListening = false;
		bool m_IsRecieving = false;
	};
}