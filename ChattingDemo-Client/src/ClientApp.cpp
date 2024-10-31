#include "ClientApp.h"

#include <Ramensoup/Core/Entrypoint.h>

ClientApp::ClientApp()
{
	//RS_LOG("Hello Client!");

}


Ramensoup::Application* Ramensoup::CreateApplication()
{
	return new ClientApp();
}
