#include "pch.h"
#include "WinsockSocket.h"

#pragma comment(lib, "ws2_32.lib") // Link with Winsock library

namespace RamenNetworking
{
	WinsockSocket::WinsockSocket()
		:m_Socket(INVALID_SOCKET)
	{
		m_Socket = socket(AF_INET, SOCK_STREAM, 0); // Address family, protocol type, protocol name
		if (m_Socket == INVALID_SOCKET)
		{
			RNET_LOG_ERROR("Socket creation failed");
		}
	}
	WinsockSocket::WinsockSocket(SOCKET nativeSocket)
		:m_Socket(nativeSocket)
	{
	}

	WinsockSocket::~WinsockSocket()
	{
		if (m_Socket != INVALID_SOCKET)
			Close();
	}


	void WinsockSocket::Close()
	{
		closesocket(m_Socket);
		m_Socket = INVALID_SOCKET;
	}

	Result WinsockSocket::Recv(char* buffer, uint32_t bufferLength)
	{
		ASSERT(m_Socket != INVALID_SOCKET);
		ASSERT(bufferLength <= std::numeric_limits<int>::max());
		// TODO : Check if buffer length < length

		auto msgLength = recv(m_Socket, buffer, bufferLength, 0);
		if (msgLength < 0)
		{
			RNET_LOG_ERROR("Failed to send data to server");
			return Result::Fail;
		}
		return Result::Success;
	}

	Result WinsockSocket::Send(const char* buffer, uint32_t msgSize)
	{
		ASSERT(m_Socket != INVALID_SOCKET);
		ASSERT(msgSize <= std::numeric_limits<int>::max());
		// TODO : Check if buffer length >= length

		auto sentLength = send(m_Socket, buffer, msgSize, 0);
		if (sentLength < 0)
		{
			RNET_LOG_ERROR("Failed to send data to server");
			return Result::Fail;
		}
		return Result::Success;
	}
}