#pragma once
#include <Ramensoup.h>

#include "GameLayer.h"

class ClientApp : public Ramensoup::Application
{
public:
	ClientApp();

private:
	std::shared_ptr<GameLayer> m_GameLayer;
};