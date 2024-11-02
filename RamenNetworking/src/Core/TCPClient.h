#pragma once

#include <thread>
#include <queue>
#include <atomic>

#include "Networking/TCPSocket.h"
#include "Utilities/MessageQueue.h"
#include "Defs.h"

namespace RamenNetworking
{
	class TCPClient
	{
	public:
		enum class Status { Disconnected, ConnectingToServer, Connected, ConnectionFailed, Disconnecting };

		static constexpr size_t MAX_MESSAGE_COUNT = 32;
		using ConnectedToServerCallback = std::function<void()>;

	public:
		TCPClient(size_t messageSize = DEFAULT_MESSAGE_SIZE, size_t messageQueueSize = DEFAULT_MESSAGE_QUEUE_SIZE);
		~TCPClient();

		TCPClient(const TCPClient&) = delete;
		TCPClient& operator=(const TCPClient&) = delete;

		Result Init();
		void SetConnectedToServerCallback(const ConnectedToServerCallback& connectedToServerCallback);
		void ConnectToServer(const Address& serverAddress);
		Result SendMessageToServer(char* buffer, uint32_t bufferSize = DEFAULT_MESSAGE_SIZE);
		void Disconnect();

		bool TryPollMessage(std::vector<char>& message);

		Status GetStatus() const { return m_Status.load(std::memory_order_acquire); }

	private:
		void NetworkLoop();

	private:
		const size_t m_MessageSize;
		size_t m_MessageQueueSize; // This is not const because this might provide resizing methods in the future.
		MessageQueue<std::vector<char>> m_MessageQueue;

		TCPSocket m_Socket{};
		std::thread m_NetworkThread{};
		Address m_ServerAddress{};

		std::atomic<bool> m_IsRunning = false; // IsRunning is set by main thread
		std::atomic<Status> m_Status = Status::Disconnected; // Status is set by network thread

		ConnectedToServerCallback m_ConnectedToServerCallback;
	};
}