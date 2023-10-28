#include "EditorLayer.h"
#include <glm/gtc/type_ptr.hpp>

namespace Editor {
	void EditorLayer::OnAttach()
	{
		m_FrameBuffer.reset(new Core::FrameBuffer);
		m_FrameBuffer->Init(m_ViewPortSize.x, m_ViewPortSize.y);
		
		m_Scene.reset(new Core::Scene("SandBox"));
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
				if (ImGui::MenuItem("Mouse"))
				{
					Core::Entity mouse(m_Scene);
					mouse.AddComponent<Core::TagComponent>("Mouse");
					mouse.AddComponent<Core::TransformComponent>(glm::vec3(0.0, 0.0, 0.0), glm::vec3(100.0, 100.0, 100.0 ));
				}

				ImGui::EndMenu();
			}

			ImGui::EndPopup();
	
		}

		for (auto& i : m_Scene->GetEntities<Core::TagComponent>())
		{
			ImGui::PushID((uint32_t)i);

			Core::TagComponent& tag = m_Scene->GetComponent<Core::TagComponent>(i);

			if (ImGui::Selectable(tag.Tag.c_str(), i == m_Scene->GetSelectedEntity()))
				m_Scene->SetSelectedEntity(i);

			ImGui::PopID();
		}

		ImGui::End();

		ImGui::Begin("Config");

		if (m_Scene->GetSelectedEntity() != entt::null)
		{
			Core::TagComponent& tag = m_Scene->GetComponent<Core::TagComponent>(m_Scene->GetSelectedEntity());
			ImGui::Text(((std::string)"Tag: " + tag.Tag).c_str());

			ImGui::Spacing();

			Core::TransformComponent& transform = m_Scene->GetComponent<Core::TransformComponent>(m_Scene->GetSelectedEntity());
			ImGui::DragFloat3("Position: ", glm::value_ptr(transform.Position), 1.0f);
			ImGui::DragFloat3("Scale: ", glm::value_ptr(transform.Scale), 1.0f);			
		}

		ImGui::End();
	}
}