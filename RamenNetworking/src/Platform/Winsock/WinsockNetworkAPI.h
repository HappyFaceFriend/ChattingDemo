#pragma once

#include <winsock.h>

#include "Networking/NetworkAPI.h"

namespace RamenNetworking
{
	class WinsockNetworkAPI : public NetworkAPIInstance
	{
	public:
		WinsockNetworkAPI() = default;
		virtual ~WinsockNetworkAPI();

		virtual Result Init() override;
		virtual bool IsValid() const override { return m_WSAStatus == WSAStatus::InitSuccessed; }

	private:
		enum class WSAStatus { NotInitialized, InitSuccessed, InitFailed };

		WSAStatus m_WSAStatus = WSAStatus::NotInitialized;
		WSADATA m_WSAData{};
	};
}