#include "pch.h"
#include "Networking/Socket.h"

#pragma comment(lib, "ws2_32.lib") // Link with Winsock library

namespace RamenNetworking
{
	Socket::Socket()
		:m_RawSocket(INVALID_SOCKET)
	{
	}

	Socket::Socket(RawSocketType rawSocket)
		:m_RawSocket(rawSocket)
	{
	}

	Socket::~Socket()
	{
		if (m_RawSocket != INVALID_SOCKET)
			Close();
	}

	Result Socket::Init()
	{
		m_RawSocket = socket(AF_INET, SOCK_STREAM, 0); // Address family, protocol type, protocol name
		if (m_RawSocket == INVALID_SOCKET)
		{
			auto errorCode = WSAGetLastError();
			RNET_LOG_ERROR("Socket creation failed. WSAErrorCode: {0}", errorCode);
			return Result::Fail;
		}
		return Result::Success;
	}

	bool Socket::IsValid() const
	{ 
		return m_RawSocket != INVALID_SOCKET;
	}

	void Socket::Close()
	{
		closesocket(m_RawSocket);
		m_RawSocket = INVALID_SOCKET;
	}

	Result Socket::Recv(char* buffer, uint32_t bufferLength)
	{
		ASSERT(m_RawSocket != INVALID_SOCKET);
		ASSERT(bufferLength <= std::numeric_limits<int>::max());
		// TODO : Check if buffer length < length

		auto msgLength = recv(m_RawSocket, buffer, bufferLength, 0);
		if (msgLength < 0)
		{
			auto errorCode = WSAGetLastError();
			RNET_LOG_ERROR("Failed to recieve data. WSAErrorCode: {0}", errorCode);
			return Result::Fail;
		}
		return Result::Success;
	}

	Result Socket::Send(const char* buffer, uint32_t msgSize)
	{
		ASSERT(m_RawSocket != INVALID_SOCKET);
		ASSERT(msgSize <= std::numeric_limits<int>::max());
		// TODO : Check if buffer length >= length

		auto sentLength = send(m_RawSocket, buffer, msgSize, 0);
		if (sentLength < 0)
		{
			auto errorCode = WSAGetLastError();
			RNET_LOG_ERROR("Failed to recieve data. WSAErrorCode: {0}", errorCode);
			return Result::Fail;
		}
		return Result::Success;
	}
}