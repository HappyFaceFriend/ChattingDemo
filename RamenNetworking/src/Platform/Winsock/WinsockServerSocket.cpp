#include "pch.h"
#include "WinsockServerSocket.h"

#include "WinsockClientSocket.h"

#pragma comment(lib, "ws2_32.lib") // Link with Winsock library

namespace RamenNetworking
{
	WinsockServerSocket::WinsockServerSocket(SOCKET nativeSocket)
		: WinsockSocket(nativeSocket)
	{

	}
	Result WinsockServerSocket::Bind(const Address& serverAddress)
	{
		ASSERT(m_Socket != INVALID_SOCKET);
		// TODO : check serverAddress validity

		sockaddr_in serverAddr; // IP Address
		serverAddr.sin_family = AF_INET; // Address family
		serverAddr.sin_addr.s_addr = inet_addr(serverAddress.IPAddress.c_str());
		serverAddr.sin_port = htons(serverAddress.PortNumber);

		auto result = bind(m_Socket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr));
		if (result == SOCKET_ERROR)
		{
			RNET_LOG_ERROR("Binding address {0}:{1} failed.", serverAddress.IPAddress, serverAddress.PortNumber);
			return Result::Fail;
		}
		return Result::Success;
	}

	Result WinsockServerSocket::Listen(uint32_t maxQueueLength)
	{
		ASSERT(m_Socket != INVALID_SOCKET);
		ASSERT(maxQueueLength <= std::numeric_limits<int>::max());

		auto result = listen(m_Socket, maxQueueLength);
		if (result == SOCKET_ERROR)
		{
			RNET_LOG_ERROR("Putting into listening state failed.");
			return Result::Fail;
		}
		return Result::Success;
	}

	ServerSocket::AcceptResult WinsockServerSocket::Accept()
	{
		SOCKET clientSocket;
		struct sockaddr_in clientAddr;
		int clientAddrSize = sizeof(clientAddr);

		clientSocket = accept(m_Socket, reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrSize);
		if (clientSocket == INVALID_SOCKET)
		{
			RNET_LOG_ERROR("Accepting new socket failed.");
			return { nullptr, Address() };
		}
		return { std::make_unique<WinsockClientSocket>(clientSocket), {inet_ntoa(clientAddr.sin_addr), clientAddr.sin_port} };
	}
}