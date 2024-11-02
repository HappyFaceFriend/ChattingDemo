#include "GameView.h"

#pragma warning(push)
#pragma warning(disable:4819 4005 4996)
#include <imgui.h>
#pragma warning(pop)

#include "ImGuiArithmetics.h"

using namespace ImGuiArithmetics;

GameView::GameView()
{
	Ramensoup::FrameBufferSpecification fbSpec;
	fbSpec.Width = 1280;
	fbSpec.Height = 720;
	m_FrameBuffer = Ramensoup::FrameBuffer::Create(fbSpec);
	m_FrameBuffer->Unbind(); // TODO: Ramensoup should unbind this

	// Scene construction
	m_Scene = std::make_shared<Ramensoup::Scene>();
	auto camera = m_Scene->CreateEntity("Main Camera");
	camera.AddComponent<Ramensoup::CameraComponent>();
	camera.GetComponent<Ramensoup::TransformComponent>().Position = glm::vec3(0, 0, -5);
}
void GameView::OnUpdate()
{
	RenderScene();
}
void GameView::RenderScene()
{
	Ramensoup::TimeProfiler::Begin("GameView Single Frame Render");
	m_FrameBuffer->Bind();
	Ramensoup::Renderer::Clear();
	m_Scene->RenderMeshes();
	m_FrameBuffer->Unbind();
	Ramensoup::TimeProfiler::End("GameView Single Frame Render");
}
void GameView::OnImGuiRender()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
	ImGui::Begin("Game View");
	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	if ((m_ViewportSize != viewportPanelSize) && viewportPanelSize.x > 0 && viewportPanelSize.y > 0)
	{
		m_ViewportSize = viewportPanelSize;
		m_FrameBuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
		m_FrameBuffer->Unbind(); // TODO: Ramensoup should unbind this
		m_Scene->OnViewportResize(viewportPanelSize.x, viewportPanelSize.y);
	}

	uint64_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
	ImGui::Image(reinterpret_cast<ImTextureID>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
	ImGui::PopStyleVar();
	ImGui::End();

}