#include "EditorLayer.h"
#include <glm/gtc/type_ptr.hpp>

namespace Editor {
	void EditorLayer::OnAttach()
	{
		m_FrameBuffer.reset(new Core::FrameBuffer);
		m_FrameBuffer->Init(m_ViewPortSize.x, m_ViewPortSize.y);
		
		m_Scene.reset(new Core::Scene("SandBox"));

		m_PerspectiveCamera.reset(new Camera3D{0.0});
		m_PerspectiveCamera->position = Vector3({ 10.0f, 10.0f, 10.0f });
		m_PerspectiveCamera->target = Vector3({ 0.0f, 0.0f, 0.0f });
		m_PerspectiveCamera->up = Vector3({ 0.0f, 1.0f, 0.0f });
		m_PerspectiveCamera->fovy = 45.0f;
		m_PerspectiveCamera->projection = CAMERA_PERSPECTIVE;

		m_World.reset(new Simulation::World);
	}

	void EditorLayer::OnUpdate()
	{
		m_FrameBuffer->Bind();

		ClearBackground(GRAY);

		if (!m_IsWorldEditEnabled)
		{
			UpdateCamera(m_PerspectiveCamera.get(), CAMERA_ORBITAL);
		
			BeginMode3D(*m_PerspectiveCamera);

			uint8_t tileArrayIndex = 0;
			for (int k = 0; k < m_World->GetLayers().size(); k++)
			{
				for (int i = -5; i < 5; i++)
				{
					for (int j = -5; j < 5; j++)
					{
						Simulation::WorldTile& tile = m_World->GetTiles(k)[tileArrayIndex];

						if (tile.Type == Simulation::TileType::Sand)
						{
							DrawCube({(float)i, (float)k, (float)j}, 1.0f, 1.0f, 1.0f, YELLOW);
							DrawCubeWires({(float)i, (float)k, (float)j}, 1.0f, 1.0f, 1.0f, GOLD);
						}
						else if (tile.Type == Simulation::TileType::Water)
						{
							DrawCube({(float)i, (float)k, (float)j}, 1.0f, 1.0f, 1.0f, BLUE);
							DrawCubeWires({(float)i, (float)k, (float)j}, 1.0f, 1.0f, 1.0f, DARKBLUE);
						}
						else if (tile.Type == Simulation::TileType::Grass)
						{
							DrawCube({(float)i, (float)k, (float)j}, 1.0f, 1.0f, 1.0f, GREEN);
							DrawCubeWires({(float)i, (float)k, (float)j}, 1.0f, 1.0f, 1.0f, DARKGREEN);
						}

						tileArrayIndex++;
					}
				}

				tileArrayIndex = 0;
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

			for (auto& i : m_World->GetTiles(m_SelectedLayer))
			{
				if (i.Type == Simulation::TileType::Sand)
					DrawRectangle(i.XPosition, i.YPositon, 60, 60, YELLOW);
				else if (i.Type == Simulation::TileType::Water)
					DrawRectangle(i.XPosition, i.YPositon, 60, 60, BLUE);
				else if (i.Type == Simulation::TileType::Grass)
					DrawRectangle(i.XPosition, i.YPositon, 60, 60, GREEN);
			}

			for (int i = 0; i < 10; i++)
			{
				for (int j = 0; j < 10; j++)
				{
					if (CheckCollisionPointRec(GetMousePosition(), { ((m_ViewPortSize.x / 2) - 300) + 60 * i, ((m_ViewPortSize.y / 2) - 300) + 60 * j, 60, 60 }))
					{
						if(m_SelectedTileType == Simulation::TileType::Sand)
							DrawRectangleLines(((m_ViewPortSize.x / 2) - 300) + 60 * i, ((m_ViewPortSize.y / 2) - 300) + 60 * j, 60, 60, YELLOW);
						else if(m_SelectedTileType == Simulation::TileType::Water)
							DrawRectangleLines(((m_ViewPortSize.x / 2) - 300) + 60 * i, ((m_ViewPortSize.y / 2) - 300) + 60 * j, 60, 60, BLUE);
						else if(m_SelectedTileType == Simulation::TileType::Grass)
							DrawRectangleLines(((m_ViewPortSize.x / 2) - 300) + 60 * i, ((m_ViewPortSize.y / 2) - 300) + 60 * j, 60, 60, GREEN);

						if (IsMouseButtonDown(0))
						{
							m_World->PushTile(m_SelectedLayer, i * 10 + j, Simulation::WorldTile{((m_ViewPortSize.x / 2) - 300) + 60 * i, ((m_ViewPortSize.y / 2) - 300) + 60 * j, m_SelectedTileType});
						}
					}
					else
					{
						DrawRectangleLines(((m_ViewPortSize.x / 2) - 300) + 60 * i, ((m_ViewPortSize.y / 2) - 300) + 60 * j, 60, 60, DARKGRAY);
					}	
				}
			}
		}

		SetMouseOffset(0, 0);

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

					if (!path.empty())
						m_World->LoadWorld(path);
				}

				if (ImGui::MenuItem("Save As.."))
				{
					std::string path = Core::FilesystemWindow::SaveFile("YAML (*.yml)\0*.yml\0");

					if(!path.empty())
						m_World->SaveWorld(path);
				}

				ImGui::EndPopup();
			}

			if (ImGui::MenuItem("World Editor"))
				m_IsWorldEditEnabled = !m_IsWorldEditEnabled;

			if (ImGui::Button("Run", { 30, 15 }))
				m_IsRunning = !m_IsRunning;

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

		if(m_IsRunning)
		{ 
			for (int i = 0; i < m_MouseToSpawn; i++)
			{
				float xPosition = GetRandomValue(-5, 4);
				float yPosition = GetRandomValue(-5, 4);
				float zPosition = 0;

				float arrayPosition = (xPosition + 5) * 10 + (yPosition + 5);

				while (m_World->GetLayers()[zPosition != m_World->GetLayers().size() - 1? zPosition + 1: zPosition].Tiles[arrayPosition].Type != Simulation::TileType::None)
				{
					zPosition++;
				}

				while (m_World->GetLayers()[zPosition].Tiles[arrayPosition].Type == Simulation::TileType::Water)
				{
					xPosition = GetRandomValue(-5, 4);
					yPosition = GetRandomValue(-5, 4);
					arrayPosition = (xPosition + 5) * 10 + (yPosition + 5);
				}

				Core::Entity mouse(m_Scene);
				mouse.AddComponent<Core::TagComponent>("Mouse");
				mouse.AddComponent<Core::TransformComponent>(glm::vec3(xPosition, 0.8f + zPosition, yPosition), glm::vec3(0.5, 0.5, 0.5));
			}
			m_MouseToSpawn = 0;
		}

		ImGui::End();

		ImGui::Begin("Scene Elements");

		m_SceneEntitiesSelectorWidth = ImGui::GetWindowSize().x;
	
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

		if (!m_IsWorldEditEnabled)
		{
			if (m_Scene->GetSelectedEntity() != entt::null)
			{
				Core::TagComponent& tag = m_Scene->GetComponent<Core::TagComponent>(m_Scene->GetSelectedEntity());
				ImGui::Text(((std::string)"Tag: " + tag.Tag).c_str());

				ImGui::Spacing();

				Core::TransformComponent& transform = m_Scene->GetComponent<Core::TransformComponent>(m_Scene->GetSelectedEntity());
				ImGui::DragFloat3("Position", glm::value_ptr(transform.Position), 0.2f, -5.0f, 4.0f);
				ImGui::DragFloat3("Scale", glm::value_ptr(transform.Scale), 0.2f, -5.0f, 4.0f);			
			}
		}
		else
		{
			if (ImGui::Selectable("Sand", m_SelectedTileType == Simulation::TileType::Sand))
				m_SelectedTileType = Simulation::TileType::Sand;
			else if (ImGui::Selectable("Grass", m_SelectedTileType == Simulation::TileType::Grass))
				m_SelectedTileType = Simulation::TileType::Grass;
			else if (ImGui::Selectable("Water", m_SelectedTileType == Simulation::TileType::Water))
				m_SelectedTileType = Simulation::TileType::Water;

		}

		ImGui::End();

		if (m_IsWorldEditEnabled)
		{
			ImGui::Begin("Layers");

			uint8_t index = 0;
			for (auto& layer : m_World->GetLayers())
			{
				ImGui::PushID(index);

				if (ImGui::Selectable(((std::string)"Layer" + std::to_string(index)).c_str(), m_SelectedLayer == index))
					m_SelectedLayer = index;

				index++;
				ImGui::PopID();
			}

			if (ImGui::Button("Add Layer"))
			{
				Simulation::WorldLayer newLayer;
				m_World->PushLayer(newLayer);
			}

			ImGui::End();
		}
		else
		{ 
			ImGui::Begin("Animals");

			ImGui::Spacing();
			ImGui::DragInt("Mouses", &m_MouseToSpawn, 1.0, 0, 100);
			ImGui::Spacing();

			ImGui::End();	
		}
	}
}