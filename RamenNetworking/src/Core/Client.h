#pragma once

#include <thread>
#include <queue>

#include "Networking/ClientSocket.h"

namespace RamenNetworking
{
	class Client
	{
	public:
		Client();
		~Client();

		Client(const Client&) = delete;
		Client& operator=(const Client&) = delete;

		Result Init();
		void ConnectToServer(const Address& serverAddress);
		Result SendMessageToServer(char* buffer, uint32_t bufferSize);

		std::queue<std::vector<char>>& GetMessageQueue() { return m_MessageQueue;  }

	private:
		void NetworkLoop();

	private:
		ClientSocket m_Socket{};
		std::thread m_NetworkThread{};
		Address m_ServerAddress{};

		// TEMP: This should be thread safe
		std::queue<std::vector<char>> m_MessageQueue{};

		bool m_IsRunning = false;

		// TODO: Maintain connection status
	};
}