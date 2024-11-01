#pragma once

#include <cstdint>

class ImGuiUtils
{
public:
	ImGuiUtils() = delete;

	static bool InputText(const char* label, char* buffer, size_t bufferSize);
};