#pragma once

#include <winsock.h>

#include "Networking/Socket.h"

namespace RamenNetworking
{
	class WinsockSocket : public virtual Socket
	{
	public:
		WinsockSocket();
		WinsockSocket(SOCKET nativeSocket);
		virtual ~WinsockSocket();

		virtual Result Init() override;
		virtual void Close() override;
		virtual Result Recv(char* buffer, uint32_t bufferSize) override;
		virtual Result Send(const char* buffer, uint32_t msgSize) override;

	protected:
		SOCKET m_Socket;
	};
}