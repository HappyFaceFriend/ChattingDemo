#pragma once

#ifdef RNET_PLATFORM_WINDOWS
	#include <winsock2.h>
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
	public:
		enum class RecieveResult { Success, Fail, Timedout, Disconnected };
		enum class SendResult { Success, Fail, Timedout, Disconnected };
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

		// TODO : Change these to take in RamenNetworking::Buffer or something ?
		RecieveResult RecieveNoTimeout(char* buffer, uint32_t bufferSize);
		// Setting timeoutMilliseconds to 0 means removing timeout (wait forever)
		RecieveResult Recieve(char* buffer, uint32_t bufferSize, uint32_t timeoutMilliseconds = 0);

		SendResult SendNoTimeout(const char* buffer, uint32_t msgSize);
		// Setting timeoutMilliseconds to 0 means removing timeout (wait forever)
		SendResult Send(const char* buffer, uint32_t msgSize, uint32_t timeoutMilliseconds = 0);

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
