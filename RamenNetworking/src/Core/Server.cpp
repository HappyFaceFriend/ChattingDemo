#include "pch.h"
#include "Server.h"

#include <algorithm>

#include "Networking/NetworkAPI.h"

namespace RamenNetworking
{
	Server::ClientID Server::s_NextClientID = 0;

	Server::Server(size_t messageSize, size_t messageQueueSize)
		:m_MessageSize(messageSize), m_MessageQueueSize(messageQueueSize), m_MessageQueue(messageQueueSize)
	{
	}

	Server::~Server()
	{
		m_IsListening = false;
		m_ListenThread.join();
		for (auto& [clientID, clientInfo] : m_ClientInfos)
			DisconnectClient(clientID);
		for(auto& [clientID, clientThread] : m_ClientThreads)
			clientThread.join();
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

		auto result = m_ServerSocket.Init();
		if (result == Result::Fail)
		{
			RNET_LOG_ERROR("Failed socket initialization.");
			return Result::Fail;
		}

		m_ServerSocket.Bind(m_ServerAddress);
		if (result == Result::Fail)
		{
			RNET_LOG_ERROR("Failed to bind address to socket.");
			return Result::Fail;
		}

		return Result::Success;
	}

	void Server::StartListening(const ClientAcceptedCallback& clientAcceptedCallback)
	{
		ASSERT(m_ServerSocket.IsValid());

		m_ClientAcceptedCallback = clientAcceptedCallback;

		m_IsListening = true;
		m_ListenThread = std::thread([this]() { ListenThreadFunc(m_ClientAcceptedCallback); });

	}
	void Server::ListenThreadFunc(const ClientAcceptedCallback& clientAcceptedCallback)
	{
		m_ServerSocket.Listen(10);
		while (m_IsListening)
		{
			RNET_LOG("Listening..");
			auto& clientInfo = m_ServerSocket.Accept();
			if (!clientInfo.clientSocket.IsValid())
			{
				RNET_LOG_ERROR("Accepting a client failed.");
			}
			else
			{
				RNET_LOG("Accepting a client.");
				std::unique_lock<std::shared_mutex> lock(m_ClientInfosLock);

				auto clientID = s_NextClientID++;
				m_ClientInfos[clientID] = {
					clientID,
					std::move(clientInfo.clientSocket),
					clientInfo.clientAddress,
					true
				};
				m_ClientThreads[clientID] = std::thread([this, clientID = clientID]() { RecieveFromClientThreadFunc(clientID); });

				m_ClientAcceptedCallback(clientInfo.clientAddress, clientID);
			}
		}
	}
	void Server::RecieveFromClientThreadFunc(ClientID clientID)
	{
		auto& clientInfo = m_ClientInfos[clientID];
		while (clientInfo.isConnected)
		{
			char messageBuffer[1024] = "";
			auto result = clientInfo.clientSocket.Recv(messageBuffer, m_MessageSize);
 			if (result == Result::Success)
			{
				// TEMP: Message should be a struct and contain id information
				auto pushResult = m_MessageQueue.TryPush(Message{ clientID, messageBuffer });
				if (!pushResult)
					RNET_LOG_WARN("Message is dropped because message queue was full.");
			}
			else
			{
				RNET_LOG_ERROR("Error reading data from {0}:{1}", clientInfo.address.IPAddress, clientInfo.address.PortNumber);
				DisconnectClient(clientID);
				break;
			}
		}
		m_ClientThreads.erase(clientID);
		m_ClientInfos.erase(clientID);
	}
	Result Server::SendMessageToAllClients(const char* buffer, uint32_t bufferSize)
	{
		std::shared_lock<std::shared_mutex> lock(m_ClientInfosLock);
		for (auto& [clientID, clientInfo] : m_ClientInfos)
		{
			auto result = clientInfo.clientSocket.Send(buffer, bufferSize);
			if (result == Result::Fail)
			{
				RNET_LOG_ERROR("Error sending data to {0}:{1}", clientInfo.address.IPAddress, clientInfo.address.PortNumber);
				DisconnectClient(clientID);
			}
		}
		// TODO: Change return type to know each result
		return Result::Success;
	}
	bool Server::TryPollMessage(Message& message)
	{
		auto result = m_MessageQueue.TryPop(message);
		return result;
	}
	void Server::DisconnectClient(ClientID clientID)
	{
		m_ClientInfos[clientID].isConnected = false;
	}
}