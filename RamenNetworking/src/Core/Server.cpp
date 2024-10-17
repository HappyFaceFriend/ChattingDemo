#include "pch.h"
#include "Server.h"

#include <algorithm>

#include "Networking/NetworkAPI.h"

namespace RamenNetworking
{
	Server::Server(size_t messageSize, size_t messageQueueSize)
		:m_MessageSize(messageSize), m_MessageQueueSize(messageQueueSize), m_MessageQueue(messageQueueSize)
	{
	}

	Server::~Server()
	{
		m_IsListening = false;
		m_IsRecieving = false;
		m_ListenThread.join();
		m_RecieveThread.join();
	}

	Result Server::Init(const Address& serverAddress)
	{
		// TODO: Validate serverAddress
		m_ServerAddress = serverAddress;

		if (!NetworkAPI::IsValid())
			NetworkAPI::Init();
		if (!NetworkAPI::IsValid())
		{
			RNET_LOG_ERROR("Failed Initializing NetworkAPI.");
			return Result::Fail;
		}

		auto result = m_Socket.Init();
		if (result == Result::Fail)
		{
			RNET_LOG_ERROR("Failed socket initialization.");
			return Result::Fail;
		}

		m_Socket.Bind(m_ServerAddress);
		if (result == Result::Fail)
		{
			RNET_LOG_ERROR("Failed to bind address to socket.");
			return Result::Fail;
		}

		return Result::Success;
	}

	void Server::StartListening(const ClientAcceptedCallback& clientAcceptedCallback)
	{
		ASSERT(m_Socket.IsValid());

		m_ClientAcceptedCallback = clientAcceptedCallback;

		m_IsListening = true;
		m_ListenThread = std::thread([this]() { ListenThreadFunc(m_ClientAcceptedCallback); });

		m_IsRecieving = true;
		m_RecieveThread = std::thread([this]() { RecieveThreadFunc(); });
	}
	void Server::ListenThreadFunc(const ClientAcceptedCallback& clientAcceptedCallback)
	{
		m_Socket.Listen(10);
		while (m_IsListening)
		{
			RNET_LOG("Listening..");
			auto& clientInfo = m_Socket.Accept();
			if (!clientInfo.clientSocket.IsValid())
			{
				RNET_LOG_ERROR("Accepting a client failed.");
			}
			else
			{
				RNET_LOG("Accepting a client.");
				std::unique_lock<std::shared_mutex> lock(m_ClientInfosLock);
				auto clientAddress = clientInfo.clientAddress;
				m_ClientInfos.emplace_back(std::move(clientInfo.clientSocket),std::move(clientInfo.clientAddress));
				m_ClientAcceptedCallback(clientAddress);
			}
		}
	}
	void Server::RecieveThreadFunc()
	{
		while (m_IsRecieving)
		{
			std::shared_lock<std::shared_mutex> lock(m_ClientInfosLock);
			for (auto& [clientSocket, clientAddress] : m_ClientInfos)
			{
				std::vector<char> messageBuffer(m_MessageSize);
				auto result = clientSocket.Recv(messageBuffer.data(), m_MessageSize);
				if (result == Result::Success)
				{
					auto pushResult = m_MessageQueue.TryPush((messageBuffer));
					if (!pushResult)
						RNET_LOG_WARN("Message is dropped because message queue was full.");
				}
				else
				{
					RNET_LOG_ERROR("Error reading data from {0}:{1}", clientAddress.IPAddress, clientAddress.PortNumber);
					// TODO : Removing a client should be seperate function
					//clientSocket.Close();
				}
			}

			// Remove all clients with invalid socket
			/*m_ClientInfos.erase(
				std::remove_if(m_ClientInfos.begin(), m_ClientInfos.end(), [](ClientInfo& info) { return !info.socket.IsValid(); }),
				m_ClientInfos.end()
			);*/
		}
	}
	Result Server::SendMessageToAllClients(char* buffer, uint32_t bufferSize)
	{
		std::shared_lock<std::shared_mutex> lock(m_ClientInfosLock);
		for (auto& [clientSocket, clientAddress] : m_ClientInfos)
		{
			auto result = clientSocket.Send(buffer, bufferSize);
			if (result == Result::Fail)
			{
				RNET_LOG_ERROR("Error sending data to {0}:{1}", clientAddress.IPAddress, clientAddress.PortNumber);
				// TODO : Removing a client should be seperate function
				clientSocket.Close();
			}
		}
		// TODO: Change return type to know each result
		return Result::Success;
	}
	bool Server::TryPollMessage(std::vector<char>& message)
	{
		auto result = m_MessageQueue.TryPop(message);
		return result;
	}
}