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

		m_World.reset(new Core::World);
	}

	void EditorLayer::OnUpdate()
	{
		m_FrameBuffer->Bind();

		ClearBackground(GRAY);

		if (!m_IsWorldEditEnabled)
		{
			UpdateCamera(m_PerspectiveCamera.get(), CAMERA_ORBITAL);
		
			BeginMode3D(*m_PerspectiveCamera);

			for (auto& i : m_World->GetTiles())
			{
				if(i.Type == Core::TileType::Sand)
					DrawCube({i.XPosition, -0.5, i.YPositon}, 1.0f, 1.0f, 1.0f, YELLOW);
				else if(i.Type == Core::TileType::Water)
					DrawCube({i.XPosition, -0.5, i.YPositon}, 1.0f, 1.0f, 1.0f, BLUE);
				else if(i.Type == Core::TileType::Grass)
					DrawCube({i.XPosition, -0.5, i.YPositon}, 1.0f, 1.0f, 1.0f, GREEN);
			}

			for (auto& i : m_Scene->GetEntities<Core::TransformComponent>())
			{
				Core::TransformComponent& transformComponent = m_Scene->GetComponent<Core::TransformComponent>(i);
				DrawCube({ transformComponent.Position.x, transformComponent.Position.y, transformComponent.Position.z}, transformComponent.Scale.x, transformComponent.Scale.y, transformComponent.Scale.z, PURPLE);
				DrawCubeWires({ transformComponent.Position.x, transformComponent.Position.y, transformComponent.Position.z}, transformComponent.Scale.x, transformComponent.Scale.y, transformComponent.Scale.z, MAGENTA);
			}

			EndMode3D();
		}
		else
		{
			SetMouseOffset(m_SceneEntitiesSelectorWidth * -1, ImGui::GetFrameHeight() * -2);

			for (int i = 0; i < 10; i++)
			{
				for (int j = 0; j < 10; j++)
				{
					DrawRectangleLines(((m_ViewPortSize.x / 2) - 300) + 60 * i, ((m_ViewPortSize.y / 2) - 300) + 60 * j, 60, 60, DARKGRAY);
				}
			}
		}

		m_FrameBuffer->UnBind();
	}

	void EditorLayer::OnImGuiRender()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowPadding = ImVec2(0, 0);

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::MenuItem("World"))
				ImGui::OpenPopup("World");

			if(ImGui::BeginPopup("World"))
			{
				if (ImGui::MenuItem("Open.."))
				{
					std::string path = Core::FilesystemWindow::OpenFile("YAML (*.yml)\0*.yml\0");

					if(!path.empty())
					{ 
						std::cout << path << std::endl;
					}
				}

				if (ImGui::MenuItem("Save As.."))
				{
					std::string path = Core::FilesystemWindow::SaveFile("YAML (*.yml)\0*.yml\0");

					if(!path.empty())
					{ 
						std::cout << path << std::endl;
					}
				}

				ImGui::EndPopup();
			}

			if (ImGui::MenuItem("World Editor"))
				m_IsWorldEditEnabled = !m_IsWorldEditEnabled;

			ImGui::EndMainMenuBar();
		}

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

		m_SceneEntitiesSelectorWidth = ImGui::GetWindowSize().x;
	
		if (ImGui::BeginPopupContextWindow())
		{
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
			ImGui::DragFloat3("Position: ", glm::value_ptr(transform.Position), 0.2f, -10.0f, 9.0f);
			ImGui::DragFloat3("Scale: ", glm::value_ptr(transform.Scale), 0.2f, -10.0f, 9.0f);			
		}

		ImGui::End();
	}
}