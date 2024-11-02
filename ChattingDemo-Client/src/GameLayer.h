#pragma once

#include <Ramensoup.h>
#include <RamenNetworking.h>

#include "Scenes/TitleView.h"

class GameLayer : public Ramensoup::Layer
{
public:
	GameLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() noexcept override;
	virtual void OnUpdate() override;
	virtual void OnImGuiUpdate() override;

	virtual bool HandleEvent(const Ramensoup::Event& e) override;

private:
	constexpr static int SERVER_PORT = 8080;
	constexpr static char SERVER_IP_ADDRESS[] = "127.0.0.1";
	constexpr static int MSG_SIZE = 1024;

	RamenNetworking::TCPClient m_ChatClient;
	char m_MsgBuffer[MSG_SIZE] = { 0 };
	std::vector<std::vector<char>> m_RecievedMessages;

	std::shared_ptr<TitleView> m_TitleView;
};