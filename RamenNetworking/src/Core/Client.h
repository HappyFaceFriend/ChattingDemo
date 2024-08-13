#pragma once

#include <thread>
#include <queue>
#include <atomic>
#include <mutex>

#include "Networking/ClientSocket.h"

namespace RamenNetworking
{
	class Client
	{
	public:
		enum class Status { Disconnected, ConnectingToServer, Connected, Disconnecting };

	public:
		Client();
		~Client();

		Client(const Client&) = delete;
		Client& operator=(const Client&) = delete;

		Result Init();
		void ConnectToServer(const Address& serverAddress);
		Result SendMessageToServer(char* buffer, uint32_t bufferSize);
		void Disconnect();
		std::vector<std::vector<char>> PollMessages();

		Status GetStatus() const { return m_Status.load(std::memory_order_acquire); }

	private:
		void NetworkLoop();

	private:
		// TEMP
		static constexpr uint32_t MSG_SIZE = 1024;

		ClientSocket m_Socket{};
		std::thread m_NetworkThread{};
		Address m_ServerAddress{};

		std::deque<std::vector<char>> m_MessageQueue{};
		std::mutex m_MessageQueueMutex;

		std::atomic<bool> m_IsRunning = false; // IsRunning is set by main thread
		std::atomic<Status> m_Status = Status::Disconnected; // Status is set by network thread
	};
}