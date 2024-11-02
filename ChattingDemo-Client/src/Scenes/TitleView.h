#pragma once

#include <Ramensoup.h>
#pragma warning(push)
#pragma warning(disable:4819 4005 4996)
#include <imgui.h>
#pragma warning(pop)

class TitleView
{
public:
	using ConnectButtonCallback = std::function<void(const char* serverIP, const char* serverPortNumber)>;
public:
	TitleView();

	void SetConnectButtonCallback(const ConnectButtonCallback& connectButtonCallback);

	void OnImGuiUpdate();
private:
	std::shared_ptr<Ramensoup::Scene> m_Scene;

	ImVec2 m_InputWindowSize = { 500, 300 };

	char m_ServerIPBuffer[16] = "127.0.0.1";
	char m_ServerPortNumberBuffer[6] = "8080";

	ConnectButtonCallback m_ConnectButtonCallback;
};