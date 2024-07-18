#include "pch.h"
#include "Client.h"

#include "Networking/NetworkAPI.h"

namespace RamenNetworking
{
	Client::Client()
	{
	}

	Client::~Client()
	{
		m_ConnectionThread.join();
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
	}
	void Client::ConnectToServer(const Address& serverAddress)
	{
		ASSERT(m_Socket.IsValid());
		m_ServerAddress = serverAddress;
		m_IsRunning = true;
		m_ConnectionThread = std::thread([this]() { NetworkLoop(); });
	}

	Result Client::SendMessageToServer(char* buffer, uint32_t bufferSize)
	{
		auto result = m_Socket.Send(buffer, bufferSize);
		if (result == RamenNetworking::Result::Fail)
		{
			RNET_LOG_ERROR("Error sending data to server.");
			return Result::Fail;
		}
		return Result::Success;
	}
	void Client::NetworkLoop()
	{
		// TEMP
		constexpr uint32_t MSG_SIZE = 1024;
		std::vector<char> messageBuffer(MSG_SIZE);

		// Make Connection with server
		auto result = m_Socket.Connect({ m_ServerAddress.IPAddress, m_ServerAddress.PortNumber });
		if (result == RamenNetworking::Result::Fail)
		{
			RNET_LOG_ERROR("Failed to connect to server.");
			return;
		}

		while (m_IsRunning)
		{
			result = m_Socket.Recv(messageBuffer.data(), MSG_SIZE);
			if (result == Result::Success)
			{
				m_MessageQueue.push(messageBuffer);
			}
			else
			{
				RNET_LOG_ERROR("Recieving data failed");
				break;
			}
		}
		m_Socket.Close();
	}
}