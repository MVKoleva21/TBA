#include "EditorLayer.h"
#include <glm/gtc/type_ptr.hpp>

namespace Editor {
	void EditorLayer::OnAttach()
	{
		m_FrameBuffer.reset(new Core::FrameBuffer);
		m_FrameBuffer->Init(m_ViewPortSize.x, m_ViewPortSize.y);
		
		m_Scene.reset(new Core::Scene("SandBox"));

		m_PerspectiveCamera.reset(new Camera3D{0.0});
		m_PerspectiveCamera->position = Vector3({ 15.0f, 15.0f, 15.0f });
		m_PerspectiveCamera->target = Vector3({ 0.0f, 0.0f, 0.0f });
		m_PerspectiveCamera->up = Vector3({ 0.0f, 1.0f, 0.0f });
		m_PerspectiveCamera->fovy = 45.0f;
		m_PerspectiveCamera->projection = CAMERA_PERSPECTIVE;
	}

	void EditorLayer::OnUpdate()
	{
		m_FrameBuffer->Bind();

		UpdateCamera(m_PerspectiveCamera.get(), CAMERA_ORBITAL);

		ClearBackground(GRAY);
	
		BeginMode3D(*m_PerspectiveCamera);

		for (auto& i : m_Scene->GetEntities<Core::TransformComponent>())
		{
			Core::TransformComponent& transformComponent = m_Scene->GetComponent<Core::TransformComponent>(i);
			DrawCube({ transformComponent.Position.x, transformComponent.Position.y, transformComponent.Position.z}, transformComponent.Scale.x, transformComponent.Scale.y, transformComponent.Scale.z, PURPLE);
			DrawCubeWires({ transformComponent.Position.x, transformComponent.Position.y, transformComponent.Position.z}, transformComponent.Scale.x, transformComponent.Scale.y, transformComponent.Scale.z, MAGENTA);
		}

		DrawGrid(100, 1.0f);

		EndMode3D();

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
					mouse.AddComponent<Core::TransformComponent>(glm::vec3(0.0, 0.5, 0.0), glm::vec3(1.0, 1.0, 1.0 ));
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
			ImGui::DragFloat3("Position: ", glm::value_ptr(transform.Position), 0.2f);
			ImGui::DragFloat3("Scale: ", glm::value_ptr(transform.Scale), 0.2f);			
		}

		ImGui::End();
	}
}