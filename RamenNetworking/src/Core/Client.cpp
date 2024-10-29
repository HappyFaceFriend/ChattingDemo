#include "pch.h"
#include "Client.h"

#include "Networking/NetworkAPI.h"

namespace RamenNetworking
{
	Client::Client(size_t messageSize, size_t messageQueueSize)
		:m_MessageSize(messageSize), m_MessageQueueSize(messageQueueSize), m_MessageQueue(messageQueueSize)
	{
	}

	Client::~Client()
	{
		if (m_Status.load(std::memory_order_acquire) != Status::Disconnected)
			Disconnect();
		m_Socket.Close();
	}

	Result Client::Init()
	{
		if (!NetworkAPI::IsValid())
			NetworkAPI::Init();
		if (!NetworkAPI::IsValid())
		{
			RNET_LOG_ERROR("Failed Initializing NetworkAPI.");
			return Result::Fail;
		}

		m_Socket.Init();
		if (!m_Socket.IsValid())
		{
			RNET_LOG_ERROR("Failed socket initialization.");
			return Result::Fail;
		}
		return Result::Success;
	}
	void Client::ConnectToServer(const Address& serverAddress)
	{
		ASSERT(m_Socket.IsValid());
		ASSERT(m_Status.load(std::memory_order_acquire) == Status::Disconnected);

		m_ServerAddress = serverAddress;
		m_IsRunning.store(true, std::memory_order_relaxed);
		m_NetworkThread = std::thread([this]() { NetworkLoop(); });
	}

	Result Client::SendMessageToServer(char* buffer, uint32_t bufferSize)
	{
		ASSERT(m_Status.load(std::memory_order_acquire) == Status::Connected);
		// TEMP
		uint32_t timeout = 10;

		auto result = m_Socket.Send(buffer, bufferSize, timeout);
		if (result == Socket::SendResult::Fail)
		{
			RNET_LOG_ERROR("Error sending data to server.");
			return Result::Fail;
		}
		// TEMP
		else if (result == Socket::SendResult::Disconnected)
		{
			return Result::Fail;
		}
		return Result::Success;
	}
	void Client::Disconnect()
	{
		ASSERT(m_Status.load(std::memory_order_acquire) != Status::Disconnected);

		m_Status.store(Status::Disconnecting, std::memory_order_release);
		m_IsRunning.store(false, std::memory_order_release); // this will signal the network thread to stop
		m_NetworkThread.join();
	}
	bool Client::TryPollMessage(std::vector<char>& message)
	{
		auto result = m_MessageQueue.TryPop(message);
		return result;
	}
	void Client::NetworkLoop()
	{
		m_Status.store(Status::ConnectingToServer, std::memory_order_release);


		// Make Connection with server
		auto result = m_Socket.Connect({ m_ServerAddress.IPAddress, m_ServerAddress.PortNumber });
		if (result == RamenNetworking::Result::Fail)
		{
			RNET_LOG_ERROR("Failed to connect to server.");
			m_Status.store(Status::Disconnected, std::memory_order_release);
			return;
		}

		m_Status.store(Status::Connected, std::memory_order_release);

		// Recieve datas
		while (m_IsRunning.load(std::memory_order_acquire))
		{
			std::vector<char> messageBuffer(m_MessageSize);
			// TEMP
			uint32_t timeout = 10;
			auto result = m_Socket.Recieve(messageBuffer.data(), m_MessageSize, timeout);
			if (result == Socket::RecieveResult::Success)
			{
				auto pushResult = m_MessageQueue.TryPush(std::move(messageBuffer));
				if (!pushResult)
					RNET_LOG_WARN("Message is dropped because message queue was full.");
			}
			else if (result == Socket::RecieveResult::Disconnected)
			{
				break;
			}
			else if (result == Socket::RecieveResult::Fail)
			{
				RNET_LOG_ERROR("Recieving data failed");
				break;
			}
		}
		m_Status.store(Status::Disconnected, std::memory_order_release);
	}

}