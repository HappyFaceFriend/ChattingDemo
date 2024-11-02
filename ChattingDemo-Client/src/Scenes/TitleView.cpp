#include "TitleView.h"

#include "ImGuiUtils.h"
#include "ImGuiArithmetics.h"

using namespace ImGuiArithmetics;

TitleView::TitleView()
{
}

void TitleView::SetConnectButtonCallback(const ConnectButtonCallback& connectButtonCallback)
{
	m_ConnectButtonCallback = connectButtonCallback;
}

void TitleView::OnImGuiUpdate()
{
	auto io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter() - m_InputWindowSize / 2);
	ImGui::SetNextWindowSize(m_InputWindowSize);
	ImGui::Begin("TitleView", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

	ImGuiUtils::InputText("Server IP", m_ServerIPBuffer, sizeof(m_ServerIPBuffer));
	ImGuiUtils::InputText("Server Port", m_ServerPortNumberBuffer, sizeof(m_ServerPortNumberBuffer));

	// TODO: Validate inputs
	if (ImGui::Button("Connect"))
	{
		m_ConnectButtonCallback(m_ServerIPBuffer, m_ServerPortNumberBuffer);
	}

	ImGui::End();
}
