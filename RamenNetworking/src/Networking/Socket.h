#pragma once

#ifdef RNET_PLATFORM_WINDOWS
	#include <winsock.h>
#endif

#include "core/NetworkUtils.h"
#include "Address.h"

namespace RamenNetworking
{
#ifdef RNET_PLATFORM_WINDOWS
	using RawSocketType = SOCKET;
#endif


	class Socket
	{
	protected:
		struct AcceptResult;

	public:
		Socket();
		Socket(RawSocketType rawSocket);
		virtual ~Socket();

		Socket(const Socket&) = delete;
		Socket& operator=(const Socket&) = delete;
		Socket(Socket&&);
		Socket& operator=(Socket&&);

		Result Init();
		bool IsValid() const;
		void Close();

		Result SetTimeout(uint32_t milliseconds);
		// TODO : Change these to take in RamenNetworking::Buffer or something ?
		Result Recv(char* buffer, uint32_t bufferSize);
		Result Send(const char* buffer, uint32_t msgSize);

		Result Listen(uint32_t maxQueueLength);
		Result Bind(const Address& serverAddress);
		AcceptResult Accept();
		Result Connect(const Address& serverAddress);

	protected:
		RawSocketType m_RawSocket;
	};

	struct Socket::AcceptResult
	{
		Socket clientSocket;
		Address clientAddress;
	};
}
