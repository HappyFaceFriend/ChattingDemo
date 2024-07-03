#include "pch.h"
#include "WinsockClientSocket.h"

namespace RamenNetworking
{
	WinsockClientSocket::WinsockClientSocket(SOCKET nativeSocket)
	: WinsockSocket(nativeSocket)
	{

	}

	Result WinsockClientSocket::Connect(const Address& serverAddress)
	{
		ASSERT(m_Socket != INVALID_SOCKET);
		// TODO : check serverAddress validity

		sockaddr_in serverAddr; // IP Address
		serverAddr.sin_family = AF_INET; // Address family
		serverAddr.sin_addr.s_addr = inet_addr(serverAddress.IPAddress.c_str());
		serverAddr.sin_port = htons(serverAddress.PortNumber);

		// Connect to server
		auto result = connect(m_Socket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr));
		if (result == SOCKET_ERROR)
		{
			RNET_LOG_ERROR("Connecting to server {0}:{1} failed.", serverAddress.IPAddress, serverAddress.PortNumber);
			return Result::Fail;
		}
		return Result::Success;
	}
}