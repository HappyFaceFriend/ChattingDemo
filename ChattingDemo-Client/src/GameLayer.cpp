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
	std::string serverIP = SERVER_IP_ADDRESS;
	unsigned short serverPort = SERVER_PORT;
#if 0
	std::cout << "CHATTING DEMO - CLIENT\n";
	std::cout << "Enter server IP: ";
	std::cin >> serverIP;
	std::cout << "Enter server Port: ";
	std::cin >> serverPort;
#endif
	m_ChatClient.Init();
	m_ChatClient.ConnectToServer({ serverIP, serverPort });

}

void GameLayer::OnDetach() noexcept
{
}

void GameLayer::OnUpdate()
{
	// Poll messages
	while (true)
	{
		std::vector<char> message;
		if (!m_ChatClient.TryPollMessage(message))
			break;
		m_RecievedMessages.emplace_back(std::move(message));
	}
}

static void ImGuiBeginDockspace();

void GameLayer::OnImGuiUpdate()
{
	ImGuiBeginDockspace(); // Begin ImGui dockspace

	auto& io = ImGui::GetIO();
	ImGui::Begin("Test Window");
	ImGui::Text("client program");

	ImGuiInputTextFlags flags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue;
	ImGui::PushID("inputMessage");
	auto inputTextReturned = ImGui::InputText("", m_MsgBuffer, MSG_SIZE, flags);
	ImGui::PopID();
	ImGui::SameLine(500);
	if (inputTextReturned || ImGui::Button("Send"))
	{
		m_ChatClient.SendMessageToServer(m_MsgBuffer, sizeof(m_MsgBuffer + 1));
		RS_LOG("Sent message: {0}", m_MsgBuffer);
		memset(m_MsgBuffer, 0, MSG_SIZE);
	}
	for (auto& message : m_RecievedMessages)
	{
		ImGui::Text(message.data());
	}
	ImGui::End();

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