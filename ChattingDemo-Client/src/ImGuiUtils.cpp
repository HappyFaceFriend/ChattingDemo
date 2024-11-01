#include "ImGuiUtils.h"

#pragma warning(push)
#pragma warning(disable:4819 4005 4996)
#include <imgui.h>
#pragma warning(pop)


bool ImGuiUtils::InputText(const char* label, char* buffer, size_t bufferSize)
{
	ImGui::Text(label);
	ImGui::SameLine();

	ImGuiInputTextFlags flags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue;
	ImGui::PushID(label);
	auto inputTextReturned = ImGui::InputText("", buffer, bufferSize, flags);
	ImGui::PopID();
    return false;
}
