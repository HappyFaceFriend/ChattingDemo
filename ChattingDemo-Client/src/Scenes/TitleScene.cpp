#include "TitleScene.h"

#pragma warning(push)
#pragma warning(disable:4819 4005 4996)
#include <imgui.h>
#pragma warning(pop)

#include "ImGuiUtils.h"

TitleScene::TitleScene()
{
}

void TitleScene::SetConnectButtonCallback(const ConnectButtonCallback& connectButtonCallback)
{
	m_ConnectButtonCallback = connectButtonCallback;
}

void TitleScene::OnImGuiUpdate()
{
	ImGui::Begin("TitleScene");

	ImGuiUtils::InputText("Server IP", m_ServerIPBuffer, sizeof(m_ServerIPBuffer));
	ImGuiUtils::InputText("Server Port", m_ServerPortNumberBuffer, sizeof(m_ServerPortNumberBuffer));

	if (ImGui::Button("Connect"))
	{
		m_ConnectButtonCallback(m_ServerIPBuffer, m_ServerPortNumberBuffer);
	}

	ImGui::End();
}
