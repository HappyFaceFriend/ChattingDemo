#pragma once

#include <Ramensoup.h>
#pragma warning(push)
#pragma warning(disable:4819 4005 4996)
#include <imgui.h>
#pragma warning(pop)

	class GameView
	{
	public:
		GameView();

		void OnUpdate();
		void OnImGuiRender();

	private:
		std::shared_ptr<Ramensoup::Scene> m_Scene;
		std::shared_ptr<Ramensoup::FrameBuffer> m_FrameBuffer;

		ImVec2 m_ViewportSize{0, 0};

		void RenderScene();
	};
