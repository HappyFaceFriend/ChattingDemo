#pragma once

#include <thread>
#include <unordered_map>
#include <shared_mutex>
#include <functional>

#include "Defs.h"
#include "Networking/TCPSocket.h"
#include "Utilities/MessageQueue.h"

namespace RamenNetworking
{
	class TCPServer
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
		TCPServer(size_t messageSize = DEFAULT_MESSAGE_SIZE, size_t messageQueueSize = DEFAULT_MESSAGE_QUEUE_SIZE);
		~TCPServer();

		TCPServer(const TCPServer&) = delete;
		TCPServer& operator=(const TCPServer&) = delete;

		Result Init(const Address& serverAddress);
		
		void StartListening(const ClientAcceptedCallback& clientAcceptedCallback);
		void StopListening();


		Result SendMessageToAllClients(const char* buffer, uint32_t bufferSize = DEFAULT_MESSAGE_SIZE);

		bool TryPollMessage(Message& message);
		void DisconnectClient(ClientID clientID);

	private:
		struct ClientConnection
		{
			ClientID id; // Mabye use GUID
			TCPSocket clientSocket;
			Address address;
			std::thread clientThread;
			bool isConnected = true; // TODO: Use enum to manage client state & change to atomic
			bool isThreadDone = false;
		};

	private:
		void ListenThreadFunc(const ClientAcceptedCallback& clientAcceptedCallback);
		void RecieveFromClientThreadFunc(ClientID clientID);
		void MainNetworkThreadFunc();

	private:
		static ClientID s_NextClientID;

		const size_t m_MessageSize;
		size_t m_MessageQueueSize; // This is not const because this might provide resizing methods in the future.
		MessageQueue<Message> m_MessageQueue;

		TCPSocket m_ServerSocket{};
		Address m_ServerAddress{};
		std::thread m_MainNetworkThread{};
		std::thread m_ListenThread{};
		
		// TEMP
		ClientAcceptedCallback m_ClientAcceptedCallback;

		std::unordered_map<ClientID, ClientConnection> m_ClientConnections;
		std::shared_mutex m_ClientInfosLock;


		std::atomic<bool> m_IsListening = false;
		std::atomic<bool> m_IsRunning = false;
	};
}