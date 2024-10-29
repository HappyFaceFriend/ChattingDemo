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

	Socket::Socket(Socket&& other)
		:m_RawSocket(other.m_RawSocket)
	{
		other.m_RawSocket = INVALID_SOCKET;
	}

	Socket& Socket::operator=(Socket&& other)
	{
		m_RawSocket = other.m_RawSocket;
		other.m_RawSocket = INVALID_SOCKET;
		return *this;
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

	Socket::RecieveResult Socket::RecieveNoTimeout(char* buffer, uint32_t bufferLength)
	{
		return Recieve(buffer, bufferLength, 0);
	}

	// Setting timeoutMilliseconds to 0 means removing timeout (wait forever)
	Socket::RecieveResult Socket::Recieve(char* buffer, uint32_t bufferLength, uint32_t timeoutMilliseconds)
	{
		ASSERT(m_RawSocket != INVALID_SOCKET);
		ASSERT(bufferLength > 0 && bufferLength <= std::numeric_limits<int>::max());

		// Set timeout (setting to 0 will remove timeout)
		if (setsockopt(m_RawSocket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&timeoutMilliseconds), sizeof(timeoutMilliseconds)) == SOCKET_ERROR)
		{
			auto errorCode = WSAGetLastError();
			RNET_LOG_ERROR("Error setting receive timeout. WSAErrorCode: {0}", errorCode);
			return RecieveResult::Fail;
		}

		auto msgLength = recv(m_RawSocket, buffer, bufferLength, 0);
		if (msgLength == 0)
		{
			return RecieveResult::Disconnected;
		}
		else if (msgLength < 0)
		{
			auto errorCode = WSAGetLastError();
			if (errorCode == WSAETIMEDOUT)
			{
				return RecieveResult::Timedout;
			}
			else
			{
				RNET_LOG_ERROR("Failed to recieve data. WSAErrorCode: {0}", errorCode);
				return RecieveResult::Fail;
			}
		}
		return RecieveResult::Success;
	}

	Socket::SendResult Socket::SendNoTimeout(const char* buffer, uint32_t msgSize)
	{
		return Send(buffer, msgSize, 0);
	}

	// Setting timeoutMilliseconds to 0 means removing timeout (wait forever)
	Socket::SendResult Socket::Send(const char* buffer, uint32_t msgSize, uint32_t timeoutMilliseconds)
	{
		ASSERT(m_RawSocket != INVALID_SOCKET);
		ASSERT(msgSize <= std::numeric_limits<int>::max());
		// TODO : Check if buffer length >= length

		// Set timeout (setting to 0 will remove timeout)
		if (setsockopt(m_RawSocket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&timeoutMilliseconds), sizeof(timeoutMilliseconds)) == SOCKET_ERROR)
		{
			auto errorCode = WSAGetLastError();
			RNET_LOG_ERROR("Error setting receive timeout. WSAErrorCode: {0}", errorCode);
			return SendResult::Fail;
		}

		auto sentLength = send(m_RawSocket, buffer, msgSize, 0);
		if (sentLength == 0)
		{
			return SendResult::Disconnected;
		}
		if (sentLength < 0)
		{
			auto errorCode = WSAGetLastError();
			if (errorCode == WSAETIMEDOUT)
			{
				return SendResult::Timedout;
			}
			else
			{
				RNET_LOG_ERROR("Failed to recieve data. WSAErrorCode: {0}", errorCode);
				return SendResult::Fail;
			}
		}
		return SendResult::Success;
	}
	Result Socket::Connect(const Address& serverAddress)
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
	Result Socket::Bind(const Address& serverAddress)
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

	Result Socket::Listen(uint32_t maxQueueLength)
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

	Socket::AcceptResult Socket::Accept()
	{
		SOCKET clientSocket;
		struct sockaddr_in clientAddr;
		int clientAddrSize = sizeof(clientAddr);

		clientSocket = accept(m_RawSocket, reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrSize);
		if (clientSocket == INVALID_SOCKET)
		{
			auto errorCode = WSAGetLastError();
			RNET_LOG_ERROR("Accepting new socket failed. WSAErrorCode: {0}", errorCode);
			return { Socket(), Address() };
		}
		return { Socket(clientSocket), {inet_ntoa(clientAddr.sin_addr), clientAddr.sin_port} };
	}
}