#pragma once

#include <thread>
#include <unordered_map>
#include <shared_mutex>
#include <functional>

#include "Defs.h"
#include "Networking/Socket.h"
#include "Utilities/MessageQueue.h"

namespace RamenNetworking
{
	class Server
	{
	public:
		using ClientID = uint32_t;
		using ClientAcceptedCallback = std::function<void(Address, ClientID)>;

		// Mabye this should contain more information
		struct Message
		{
			ClientID id;
			std::string message;
		};
	public:
		Server(size_t messageSize = DEFAULT_MESSAGE_SIZE, size_t messageQueueSize = DEFAULT_MESSAGE_QUEUE_SIZE);
		~Server();

		Server(const Server&) = delete;
		Server& operator=(const Server&) = delete;

		Result Init(const Address& serverAddress);
		void StartListening(const ClientAcceptedCallback& clientAcceptedCallback);
		Result SendMessageToAllClients(const char* buffer, uint32_t bufferSize = DEFAULT_MESSAGE_SIZE);

		bool TryPollMessage(Message& message);
		void DisconnectClient(ClientID clientID);
	private:
		struct ClientInfo
		{
			ClientID id; // Mabye use GUID
			Socket clientSocket;
			Address address;
			bool isConnected = true; // TODO: Use enum to manage client state
		};

	private:
		void ListenThreadFunc(const ClientAcceptedCallback& clientAcceptedCallback);
		void RecieveFromClientThreadFunc(ClientID clientID);

	private:
		static ClientID s_NextClientID;

		const size_t m_MessageSize;
		size_t m_MessageQueueSize; // This is not const because this might provide resizing methods in the future.
		MessageQueue<Message> m_MessageQueue;

		Socket m_ServerSocket{};
		Address m_ServerAddress{};
		std::thread m_ListenThread{};
		
		// TEMP
		ClientAcceptedCallback m_ClientAcceptedCallback;

		std::unordered_map<ClientID, ClientInfo> m_ClientInfos;
		std::unordered_map<ClientID, std::thread> m_ClientThreads;
		std::shared_mutex m_ClientInfosLock;



		// TODO: This should be thread safe
		bool m_IsListening = false;
	};
}