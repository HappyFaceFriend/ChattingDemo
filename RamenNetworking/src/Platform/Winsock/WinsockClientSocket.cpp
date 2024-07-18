#include "pch.h"
#include "Networking/ClientSocket.h"

namespace RamenNetworking
{
	ClientSocket::ClientSocket()
		: Socket()
	{
	}

	ClientSocket::ClientSocket(RawSocketType rawSocket)
		: Socket(rawSocket)
	{
	}

	ClientSocket::~ClientSocket()
	{
	}

	Result ClientSocket::Connect(const Address& serverAddress)
	{
		ASSERT(m_RawSocket != INVALID_SOCKET);
		// TODO : check serverAddress validity

		sockaddr_in serverAddr; // IP Address
		serverAddr.sin_family = AF_INET; // Address family
		serverAddr.sin_addr.s_addr = inet_addr(serverAddress.IPAddress.c_str());
		serverAddr.sin_port = htons(serverAddress.PortNumber);

		// Connect to server
		auto result = connect(m_RawSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr));
		if (result == SOCKET_ERROR)
		{
			auto errorCode = WSAGetLastError();
			RNET_LOG_ERROR("Connecting to server {0}:{1} failed. WSAErrorCode: {2}", serverAddress.IPAddress, serverAddress.PortNumber, errorCode);
			return Result::Fail;
		}
		return Result::Success;
	}
}