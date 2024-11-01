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
	m_Client.Init();
	m_Client.ConnectToServer({ serverIP, serverPort });

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
		if (!m_Client.TryPollMessage(message))
			break;
		m_RecievedMessages.emplace_back(std::move(message));
	}
}

void GameLayer::OnImGuiUpdate()
{
	ImGui::Begin("Test Window");
	ImGui::Text("client program");

	ImGuiInputTextFlags flags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue;
	ImGui::PushID("inputMessage");
	auto inputTextReturned = ImGui::InputText("", m_MsgBuffer, MSG_SIZE, flags);
	ImGui::PopID();
	ImGui::SameLine(500);
	if (inputTextReturned || ImGui::Button("Send"))
	{
		m_Client.SendMessageToServer(m_MsgBuffer, sizeof(m_MsgBuffer + 1));
		RS_LOG("Sent message: {0}", m_MsgBuffer);
		memset(m_MsgBuffer, 0, MSG_SIZE);
	}
	for (auto& message : m_RecievedMessages)
	{
		ImGui::Text(message.data());
	}
	ImGui::End();
}

bool GameLayer::HandleEvent(const Ramensoup::Event& e)
{
	return false;
}
