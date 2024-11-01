#include "ClientApp.h"

#include <Ramensoup/Core/Entrypoint.h>

ClientApp::ClientApp()
{
	//RS_LOG("Hello Client!");
	m_GameLayer = std::make_shared<GameLayer>();
	PushLayer(m_GameLayer);

}


Ramensoup::Application* Ramensoup::CreateApplication()
{
	return new ClientApp();
}
