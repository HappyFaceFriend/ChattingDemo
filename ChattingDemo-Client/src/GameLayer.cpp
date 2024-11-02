#include "GameLayer.h"

#pragma warning(push)
#pragma warning(disable:4819 4005 4996)
#include <imgui.h>
#pragma warning(pop)

GameLayer::GameLayer()
{

}

void GameLayer::OnAttach()
{
	m_TitleView = std::make_shared<TitleView>();
	m_TitleView->SetConnectButtonCallback([&](const char* serverIP, const char* serverPortNumber) {
		m_ChatClient.Init();
		m_ChatClient.ConnectToServer({ serverIP, (uint16_t)std::atoi(serverPortNumber) });
	});


}

void GameLayer::OnDetach() noexcept
{
}

void GameLayer::OnUpdate()
{
	// Poll messages
	if (m_ChatClient.GetStatus() == RamenNetworking::TCPClient::Status::Connected)
	{
		while (true)
		{
			std::vector<char> message;
			if (!m_ChatClient.TryPollMessage(message))
				break;
			m_RecievedMessages.emplace_back(std::move(message));
		}
	}
}

static void ImGuiBeginDockspace();

void GameLayer::OnImGuiUpdate()
{
	ImGuiBeginDockspace(); // Begin ImGui dockspace

	auto clientStatus = m_ChatClient.GetStatus();
	if (clientStatus == RamenNetworking::TCPClient::Status::Disconnected)
	{
		m_TitleView->OnImGuiUpdate();
	}
	else if (clientStatus == RamenNetworking::TCPClient::Status::Connected)
	{

		ImGui::Begin("Test Window", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
		ImGui::Text("client program");

		ImGuiInputTextFlags flags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue;
		ImGui::PushID("inputMessage");
		auto inputTextReturned = ImGui::InputText("", m_MsgBuffer, MSG_SIZE, flags);
		ImGui::PopID();
		ImGui::SameLine(0);
		if (inputTextReturned || ImGui::Button("Send"))
		{
			if (strcmp(m_MsgBuffer, "QUIT") == 0)
			{
				strcpy(m_MsgBuffer, "");
				m_ChatClient.Disconnect();
			}
			else
			{
				m_ChatClient.SendMessageToServer(m_MsgBuffer, sizeof(m_MsgBuffer + 1));
				RS_LOG("Sent message: {0}", m_MsgBuffer);
				memset(m_MsgBuffer, 0, MSG_SIZE);
				ImGui::SetKeyboardFocusHere(-1);
			}
		}
		for (auto& message : m_RecievedMessages)
		{
			ImGui::Text(message.data());
		}
		ImGui::End();
	}

	ImGui::End(); // End ImGui dockspace
}

bool GameLayer::HandleEvent(const Ramensoup::Event& e)
{
	return false;
}




static void ImGuiBeginDockspace()
{
	static bool dockSpaceOpen = true;

	//From imgui_demo.cpp ShowExampleAppDockSpace
	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;
	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin("DockSpace Demo", &dockSpaceOpen, window_flags);
	if (!opt_padding)
		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// Submit the DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

}