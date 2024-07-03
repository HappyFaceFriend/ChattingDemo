#pragma once

#include <string>

namespace RamenNetworking
{
	struct Address
	{
		std::string IPAddress;
		uint16_t PortNumber;
	};
}