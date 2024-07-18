#include "pch.h"
#include "Networking/ServerSocket.h"

#pragma comment(lib, "ws2_32.lib") // Link with Winsock library

namespace RamenNetworking
{
	ServerSocket::ServerSocket()
		: Socket()
	{
	}

	ServerSocket::ServerSocket(RawSocketType rawSocket)
		: Socket(rawSocket)
	{
	}

	ServerSocket::~ServerSocket()
	{
	}

	Result ServerSocket::Bind(const Address& serverAddress)
	{
		ASSERT(m_RawSocket != INVALID_SOCKET);
		// TODO : check serverAddress validity

		sockaddr_in serverAddr; // IP Address
		serverAddr.sin_family = AF_INET; // Address family
		serverAddr.sin_addr.s_addr = inet_addr(serverAddress.IPAddress.c_str());
		serverAddr.sin_port = htons(serverAddress.PortNumber);

		auto result = bind(m_RawSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr));
		if (result == SOCKET_ERROR)
		{
			auto errorCode = WSAGetLastError();
			RNET_LOG_ERROR("Binding address {0}:{1} failed. WSAErrorCode: {2}", serverAddress.IPAddress, serverAddress.PortNumber, errorCode);
			return Result::Fail;
		}
		return Result::Success;
	}

	Result ServerSocket::Listen(uint32_t maxQueueLength)
	{
		ASSERT(m_RawSocket != INVALID_SOCKET);
		ASSERT(maxQueueLength <= static_cast<uint32_t>(std::numeric_limits<int>::max()));

		auto result = listen(m_RawSocket, maxQueueLength);
		if (result == SOCKET_ERROR)
		{
			auto errorCode = WSAGetLastError();
			RNET_LOG_ERROR("Putting into listening state failed. WSAErrorCode: {0}", errorCode);
			return Result::Fail;
		}
		return Result::Success;
	}

	ServerSocket::AcceptResult ServerSocket::Accept()
	{
		SOCKET clientSocket;
		struct sockaddr_in clientAddr;
		int clientAddrSize = sizeof(clientAddr);

		clientSocket = accept(m_RawSocket, reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrSize);
		if (clientSocket == INVALID_SOCKET)
		{
			auto errorCode = WSAGetLastError();
			RNET_LOG_ERROR("Accepting new socket failed. WSAErrorCode: {0}", errorCode);
			return { ClientSocket(), Address()};
		}
		return { ClientSocket(clientSocket), {inet_ntoa(clientAddr.sin_addr), clientAddr.sin_port} };
	}
}