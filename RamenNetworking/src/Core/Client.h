#pragma once

#include <thread>
#include <queue>
#include <atomic>
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

		std::queue<std::vector<char>>& GetMessageQueue() { return m_MessageQueue;  }
		Status GetStatus() const { return m_Status.load(std::memory_order_acquire); }

	private:
		void NetworkLoop();

	private:
		ClientSocket m_Socket{};
		std::thread m_NetworkThread{};
		Address m_ServerAddress{};

		// TEMP: This should be thread safe
		std::queue<std::vector<char>> m_MessageQueue{};
		std::atomic<bool> m_IsRunning = false;

		// Status is set by network thread
		std::atomic<Status> m_Status = Status::Disconnected;
	};
}