#pragma once

#include <Ramensoup.h>

class TitleScene : Ramensoup::Scene
{
public:
	using ConnectButtonCallback = std::function<void(const char* serverIP, const char* serverPortNumber)>;
public:
	TitleScene();

	void SetConnectButtonCallback(const ConnectButtonCallback& connectButtonCallback);

	void OnImGuiUpdate();
private:
	char m_ServerIPBuffer[16] = "127.0.0.1";
	char m_ServerPortNumberBuffer[6] = "8080";

	ConnectButtonCallback m_ConnectButtonCallback;
};