#pragma once

#include "core/NetworkUtils.h"

namespace RamenNetworking
{
	class Socket
	{
	public:
		Socket() = default;
		virtual ~Socket() = default;

		Socket(const Socket&) = delete;
		Socket& operator=(const Socket&) = delete;

		virtual bool IsValid() const = 0;
		virtual void Close() = 0;

		// TODO : Change these to take in RamenNetworking::Buffer or something ?
		virtual Result Recv(char* buffer, uint32_t bufferSize) = 0;
		virtual Result Send(const char* buffer, uint32_t msgSize) = 0;
	};
}
