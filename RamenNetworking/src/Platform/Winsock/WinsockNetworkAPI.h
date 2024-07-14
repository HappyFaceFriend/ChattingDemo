#pragma once

#include "Networking/NetworkAPI.h"

#include <winsock.h>

namespace RamenNetworking
{
	class WinsockNetworkAPI : public NetworkAPI
	{
	public:
		WinsockNetworkAPI();
		virtual ~WinsockNetworkAPI();

		bool IsValid() const { return m_WSAStatus == WSAStatus::InitSuccessed; }

	private:
		enum class WSAStatus { NotInitialized, InitSuccessed, InitFailed };

		
		WSAStatus m_WSAStatus = WSAStatus::NotInitialized;
		WSADATA m_WSAData;
	};
}