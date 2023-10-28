#include "EditorLayer.h"

namespace Editor {
	void EditorLayer::OnAttach()
	{
		m_FrameBuffer.reset(new Core::FrameBuffer);
		m_FrameBuffer->Init(m_ViewPortSize.x, m_ViewPortSize.y);
		
		m_Scene.reset(new Core::Scene("SandBox"));

		Core::Entity entityOne(m_Scene);
		entityOne.AddComponent<Core::TagComponent>("Test entity one");
		entityOne.AddComponent<Core::TransformComponent>(glm::vec3(0.0, 0.0, 0.0), glm::vec3(100.0, 100.0, 100.0 ));

		Core::Entity entityTwo(m_Scene);
		entityTwo.AddComponent<Core::TagComponent>("Test entity two");
		entityTwo.AddComponent<Core::TransformComponent>(glm::vec3(150.0, 150.0, 0.0), glm::vec3(100.0, 100.0, 100.0 ));
	}

	void EditorLayer::OnUpdate()
	{
		m_FrameBuffer->Bind();

		ClearBackground(GRAY);

		for (auto& i : m_Scene->GetEntities<Core::TransformComponent>())
		{
			Core::TransformComponent& transformComponent = m_Scene->GetComponent<Core::TransformComponent>(i);
			DrawRectangle(transformComponent.Position.x, transformComponent.Position.y, transformComponent.Scale.x, transformComponent.Scale.y, PURPLE);
		}

		m_FrameBuffer->UnBind();
	}

	void EditorLayer::OnImGuiRender()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowPadding = ImVec2(0, 0);

		ImGui::Begin("Viewport");

		if (m_ViewPortSize.x != ImGui::GetContentRegionAvail().x || m_ViewPortSize.y != ImGui::GetContentRegionAvail().y)
		{
			m_ViewPortSize.x = ImGui::GetContentRegionAvail().x;
			m_ViewPortSize.y = ImGui::GetContentRegionAvail().y;

			m_FrameBuffer.reset(new Core::FrameBuffer);
			m_FrameBuffer->Init(m_ViewPortSize.x, m_ViewPortSize.y);
		}

		rlImGuiImageRenderTexture(m_FrameBuffer->GetTexture().get());

		ImGui::End();

		ImGui::Begin("Scene Elements");
	
		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::MenuItem("World"))
			{
				// Add world entity
			}

			if(ImGui::BeginMenu("Add entity"))
			{ 
				if (ImGui::MenuItem("Rabit"))
				{
					// Add rabit entity
				}

				ImGui::EndMenu();
			}

			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Config");

		ImGui::End();
	}
}