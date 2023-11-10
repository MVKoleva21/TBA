#include "EditorLayer.h"
#include <glm/gtc/type_ptr.hpp>
#include "SimulationComponents.h"
#include "Models.h"

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

		m_Models.reset(new Simulation::Models);
	}

	void EditorLayer::OnUpdate()
	{
		m_FrameBuffer->Bind();

		ClearBackground(GRAY);

		if (!m_IsWorldEditEnabled)
		{
			UpdateCamera(m_PerspectiveCamera.get(), CAMERA_ORBITAL);
		
			BeginMode3D(*m_PerspectiveCamera);

			uint8_t tileIndexX = 0;
			uint8_t tileIndexY = 0;
			for (int k = 0; k < m_World->GetLayers().size(); k++)
			{
				for (int i = -5; i < 5; i++)
				{
					for (int j = -5; j < 5; j++)
					{
						Simulation::WorldTile& tile = m_World->GetTiles(k)[tileIndexX][tileIndexY];

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

						tileIndexX++;
					}
					tileIndexY++;
					tileIndexX = 0;
				}

				tileIndexX = 0;
				tileIndexY = 0;
			}	

			for (auto& i : m_Scene->GetEntities<Core::TransformComponent>())
			{
				Core::TransformComponent& transformComponent = m_Scene->GetComponent<Core::TransformComponent>(i);
				Simulation::ColorComponent& colorComponent = m_Scene->GetComponent<Simulation::ColorComponent>(i);

				if(colorComponent.RabbitColor == Simulation::AnimalsColors::Gray)
					DrawModelEx(*m_Models->GetGrayRabbitModel(), { transformComponent.Position.x, transformComponent.Position.y, transformComponent.Position.z }, { 0.0, 1.0, 0.0 }, transformComponent.Rotation, { 0.5f, 0.5f, 0.5f }, WHITE);
				else if(colorComponent.RabbitColor == Simulation::AnimalsColors::Black)
					DrawModelEx(*m_Models->GetBlackRabbitModel(), { transformComponent.Position.x, transformComponent.Position.y, transformComponent.Position.z }, { 0.0, 1.0, 0.0 }, transformComponent.Rotation, { 0.5f, 0.5f, 0.5f }, WHITE);
				else if(colorComponent.RabbitColor == Simulation::AnimalsColors::White)
					DrawModelEx(*m_Models->GetWhiteRabbitModel(), { transformComponent.Position.x, transformComponent.Position.y, transformComponent.Position.z }, { 0.0, 1.0, 0.0 }, transformComponent.Rotation, { 0.5f, 0.5f, 0.5f }, WHITE);
			}

			//if (m_IsRunning)
			//{
			//	for (auto& i : m_Scene->GetEntities<Core::TransformComponent>())
			//	{
			//		Core::TransformComponent& transformComponent = m_Scene->GetComponent<Core::TransformComponent>(i);
			//		Simulation::TileLocation& tileLocation = m_Scene->GetComponent<Simulation::TileLocation>(i);

			//		uint32_t direction = GetRandomValue(1, 4);
			//	
			//		tileLocation.ArrayPosX = round(transformComponent.Position.z + 5);
			//		tileLocation.ArrayPosY = round(transformComponent.Position.x + 5);

			//		if (tileLocation.Layer + 1 != m_World->GetLayers().size() && m_World->GetLayers()[tileLocation.Layer + 1].Tiles[tileLocation.ArrayPosX][tileLocation.ArrayPosY].Type != Simulation::TileType::None)
			//		{
			//			transformComponent.Position.y += 1.0f;
			//			tileLocation.Layer += 1;
			//		}

			//		if (tileLocation.Layer > 0 && m_World->GetLayers()[tileLocation.Layer - 1].Tiles[tileLocation.ArrayPosX][tileLocation.ArrayPosY].Type == Simulation::TileType::None)
			//		{
			//			transformComponent.Position.y -= 1.0f;
			//			tileLocation.Layer -= 1;
			//		}
			//	}
			//}

			EndMode3D();
		}
		else
		{
			SetMouseOffset(m_SceneEntitiesSelectorWidth * -1, ImGui::GetFrameHeight() * -2);

			if (m_SelectedLayer > 0)
			{
				uint32_t opacity = 20;
				for (auto& layer : m_World->GetLayers())
				{
					for (auto& i : layer.Tiles)
					{
						for (auto& j : i)
						{
							if (j.Type == Simulation::TileType::Sand)
								DrawRectangle(j.XPosition, j.YPositon, 60, 60, Color{ 253, 249, 0, (unsigned char)opacity });
							else if (j.Type == Simulation::TileType::Water)
								DrawRectangle(j.XPosition, j.YPositon, 60, 60, Color{ 0, 121, 241, (unsigned char)opacity });
							else if (j.Type == Simulation::TileType::Grass)
								DrawRectangle(j.XPosition, j.YPositon, 60, 60, Color{ 0, 228, 48, (unsigned char)opacity });
						}	
					}

					if (opacity < 255)
						opacity += 50;
					else
						opacity = 200;
				}
			}	

			for (auto& i : m_World->GetTiles(m_SelectedLayer))
			{
				for (auto& j : i)
				{
					if (j.Type == Simulation::TileType::Sand)
						DrawRectangle(j.XPosition, j.YPositon, 60, 60, YELLOW);
					else if (j.Type == Simulation::TileType::Water)
						DrawRectangle(j.XPosition, j.YPositon, 60, 60, BLUE);
					else if (j.Type == Simulation::TileType::Grass)
						DrawRectangle(j.XPosition, j.YPositon, 60, 60, GREEN);	
				}	
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
							m_World->PushTile(m_SelectedLayer, i, j, Simulation::WorldTile{((m_ViewPortSize.x / 2) - 300) + 60 * i, ((m_ViewPortSize.y / 2) - 300) + 60 * j, m_SelectedTileType});
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

		if (m_IsRunning)
		{
			for (int j = 0; j < m_RabbitsToSpawn; j++)
			{
				for (int i = m_World->GetLayers().size() - 1; i >= 0; i--)
				{
					float xPosition = GetRandomValue(0, 9);
					float zPosition = GetRandomValue(0, 9);			

					Simulation::WorldTile currentTile = m_World->GetLayers()[i].Tiles[zPosition][xPosition];

					if (currentTile.Type == Simulation::TileType::None)
						continue;

					while (currentTile.Type == Simulation::TileType::Water || m_World->GetLayers()[i + 1].Tiles[zPosition][xPosition].Type != Simulation::TileType::None)
					{
						xPosition = GetRandomValue(0, 9);
						zPosition = GetRandomValue(0, 9);
						currentTile = m_World->GetLayers()[i].Tiles[zPosition][xPosition];

						if (currentTile.Type == Simulation::TileType::None)
							break;
					}

					if (currentTile.Type == Simulation::TileType::None)
						continue;

					uint32_t color = GetRandomValue(1, 3);
					Core::Entity rabbit(m_Scene);
					rabbit.AddComponent<Core::TagComponent>("Rabbit");
					rabbit.AddComponent<Core::TransformComponent>(glm::vec3(xPosition - 5, 0.5f + i, zPosition - 5), 0.0f);
					rabbit.AddComponent<Simulation::TileLocation>(xPosition, zPosition, i);
					rabbit.AddComponent<Simulation::ColorComponent>((Simulation::AnimalsColors)color);

					m_World->GetLayers()[i].Tiles[zPosition][xPosition].IsThereEntity = true;
				}
			}
			m_RabbitsToSpawn = 0;
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
			}
		}
		else
		{
			if (ImGui::Selectable("None", m_SelectedTileType == Simulation::TileType::None))
				m_SelectedTileType = Simulation::TileType::None;
			else if (ImGui::Selectable("Sand", m_SelectedTileType == Simulation::TileType::Sand))
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

				if (ImGui::Button("Remove Layer"))
				{
					m_World->PopLayer(index);
				}

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
			ImGui::DragInt("Rabbits", &m_RabbitsToSpawn, 1.0, 0, 100);
			ImGui::Spacing();

			ImGui::End();	
		}
	}
}