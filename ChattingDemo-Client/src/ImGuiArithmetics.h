#pragma once

#pragma warning(push)
#pragma warning(disable:4819 4005 4996)
#include <imgui.h>
#pragma warning(pop)

namespace ImGuiArithmetics
{
	ImVec2 operator+(const ImVec2& a, const ImVec2& b);
	ImVec2 operator-(const ImVec2& a, const ImVec2& b);
	ImVec2 operator/(const ImVec2& a, float f);
	ImVec2 operator*(const ImVec2& a, float f); 
	bool operator==(const ImVec2& a, const ImVec2& b);
	bool operator!=(const ImVec2& a, const ImVec2& b);
}