#include "pch.h"
#include "TCPServer.h"

#include <algorithm>

#include "Networking/NetworkAPI.h"

namespace RamenNetworking
{
	TCPServer::ClientID TCPServer::s_NextClientID = 0;

	TCPServer::TCPServer(size_t messageSize, size_t messageQueueSize)
		:m_MessageSize(messageSize), m_MessageQueueSize(messageQueueSize), m_MessageQueue(messageQueueSize)
	{
	}

	TCPServer::~TCPServer()
	{
		m_IsListening.store(false, std::memory_order_release);
		m_IsRunning.store(false, std::memory_order_release);
		for (auto& [clientID, connection] : m_ClientConnections)
			DisconnectClient(clientID);

		m_ListenThread.join();
		for (auto& [clientID, connection] : m_ClientConnections)
		{
			connection.clientThread.join();
		}
		m_MainNetworkThread.join();
	}

	Result TCPServer::Init(const Address& serverAddress)
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

	void TCPServer::StartListening(const ClientAcceptedCallback& clientAcceptedCallback)
	{
		ASSERT(m_ServerSocket.IsValid());

		m_ClientAcceptedCallback = clientAcceptedCallback;

		m_IsListening.store(true, std::memory_order_relaxed);
		m_ListenThread = std::thread([this]() { ListenThreadFunc(m_ClientAcceptedCallback); });

		if (!m_IsRunning.load(std::memory_order_acquire))
		{
			m_IsRunning.store(true, std::memory_order_relaxed);
			m_MainNetworkThread = std::thread([this]() { MainNetworkThreadFunc(); });
		}
	}

	void TCPServer::MainNetworkThreadFunc()
	{
		std::vector<ClientID> clientsToRemove;
		while (m_IsRunning.load(std::memory_order_acquire))
		{
			// TEMP
			std::this_thread::sleep_for(std::chrono::milliseconds(100));

			// Disconnect clients
			{
				std::shared_lock<std::shared_mutex> readLock(m_ClientInfosLock);
				for (auto& [clientID, connection] : m_ClientConnections)
				{
					if (connection.isThreadDone)
					{
						connection.clientThread.join();
						clientsToRemove.push_back(clientID);
					}
				}
			}
			if (!clientsToRemove.empty())
			{
				std::unique_lock<std::shared_mutex> lock(m_ClientInfosLock);
				for (auto clientID : clientsToRemove)
				{
					m_ClientConnections.erase(clientID);
				}
				clientsToRemove.clear();
			}
		}

	}
	void TCPServer::ListenThreadFunc(const ClientAcceptedCallback& clientAcceptedCallback)
	{
		m_ServerSocket.Listen(10);
		while (m_IsListening.load(std::memory_order_acquire))
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
				m_ClientConnections[clientID] = {
					clientID,
					std::move(clientInfo.clientSocket),
					clientInfo.clientAddress,
					std::thread([this, clientID = clientID]() { RecieveFromClientThreadFunc(clientID); })
				};

				m_ClientAcceptedCallback(clientInfo.clientAddress, clientID);
			}
		}
	}
	void TCPServer::RecieveFromClientThreadFunc(ClientID clientID)
	{
		auto& clientInfo = m_ClientConnections[clientID];
		// TEMP
		uint32_t timeout = 10;
		while (clientInfo.isConnected)
		{
			char messageBuffer[1024] = "";
			auto result = clientInfo.clientSocket.Recieve(messageBuffer, m_MessageSize, timeout);
 			if (result == TCPSocket::RecieveResult::Success)
			{
				// TEMP: Message should be a struct and contain id information
				auto pushResult = m_MessageQueue.TryPush(Message{ clientID, messageBuffer });
				if (!pushResult)
					RNET_LOG_WARN("Message is dropped because message queue was full.");
			}
			else if (result == TCPSocket::RecieveResult::Disconnected)
			{
				DisconnectClient(clientID);
			}
			else if (result == TCPSocket::RecieveResult::Fail)
			{
				RNET_LOG_ERROR("Error reading data from {0}:{1}", clientInfo.address.IPAddress, clientInfo.address.PortNumber);
				DisconnectClient(clientID);
				break;
			}
		}
		clientInfo.isThreadDone = true;
	}

	Result TCPServer::SendMessageToAllClients(const char* buffer, uint32_t bufferSize)
	{
		std::shared_lock<std::shared_mutex> lock(m_ClientInfosLock);
		// TEMP
		uint32_t timeout = 10;
		for (auto& [clientID, clientInfo] : m_ClientConnections)
		{
			auto result = clientInfo.clientSocket.Send(buffer, bufferSize, timeout);
			if (result == TCPSocket::SendResult::Disconnected)
			{
				DisconnectClient(clientID);
			}
			else if (result == TCPSocket::SendResult::Fail)
			{
				RNET_LOG_ERROR("Error sending data to {0}:{1}", clientInfo.address.IPAddress, clientInfo.address.PortNumber);
				DisconnectClient(clientID);
			}
		}
		// TODO: Change return type to know each result
		return Result::Success;
	}

	bool TCPServer::TryPollMessage(Message& message)
	{
		auto result = m_MessageQueue.TryPop(message);
		return result;
	}

	void TCPServer::DisconnectClient(ClientID clientID)
	{
		m_ClientConnections[clientID].isConnected = false;
	}

	void TCPServer::StopListening()
	{
		m_IsListening.store(false, std::memory_order_release);
	}
}