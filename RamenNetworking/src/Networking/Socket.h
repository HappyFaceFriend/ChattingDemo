#pragma once

#ifdef RNET_PLATFORM_WINDOWS
	#include <winsock.h>
#endif

#include "core/NetworkUtils.h"

namespace RamenNetworking
{
#ifdef RNET_PLATFORM_WINDOWS
	using RawSocketType = SOCKET;
#endif

	class Socket
	{
	public:
		Socket();
		Socket(RawSocketType rawSocket);
		virtual ~Socket();

		Socket(const Socket&) = delete;
		Socket& operator=(const Socket&) = delete;
		Socket(Socket&&) = delete;
		Socket& operator=(Socket&&) = delete;

		Result Init();
		bool IsValid() const;
		void Close();

		Result SetTimeout(uint32_t milliseconds);
		// TODO : Change these to take in RamenNetworking::Buffer or something ?
		Result Recv(char* buffer, uint32_t bufferSize);
		Result Send(const char* buffer, uint32_t msgSize);

	protected:
		RawSocketType m_RawSocket;
	};
}
