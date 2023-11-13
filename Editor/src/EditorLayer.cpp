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
				Simulation::AnimalComponent& animal = m_Scene->GetComponent<Simulation::AnimalComponent>(i);
				Core::TransformComponent& transformComponent = m_Scene->GetComponent<Core::TransformComponent>(i);
				Simulation::ColorComponent& colorComponent = m_Scene->GetComponent<Simulation::ColorComponent>(i);

				if (animal.Type == Simulation::Animals::Rabbit)
				{
					if(colorComponent.RabbitColor == Simulation::AnimalsColors::Gray)
						DrawModelEx(*m_Models->GetGrayRabbitModel(), { transformComponent.Position.x, transformComponent.Position.y, transformComponent.Position.z }, { 0.0, 1.0, 0.0 }, transformComponent.Rotation, { 0.5f, 0.5f, 0.5f }, WHITE);
					else if(colorComponent.RabbitColor == Simulation::AnimalsColors::Black)
						DrawModelEx(*m_Models->GetBlackRabbitModel(), { transformComponent.Position.x, transformComponent.Position.y, transformComponent.Position.z }, { 0.0, 1.0, 0.0 }, transformComponent.Rotation, { 0.5f, 0.5f, 0.5f }, WHITE);
					else if(colorComponent.RabbitColor == Simulation::AnimalsColors::White)
						DrawModelEx(*m_Models->GetWhiteRabbitModel(), { transformComponent.Position.x, transformComponent.Position.y, transformComponent.Position.z }, { 0.0, 1.0, 0.0 }, transformComponent.Rotation, { 0.5f, 0.5f, 0.5f }, WHITE);
				}
				else if (animal.Type == Simulation::Animals::Fox)
				{ 
					if(colorComponent.RabbitColor == Simulation::AnimalsColors::White)
						DrawModelEx(*m_Models->GetArcticFoxModel(), { transformComponent.Position.x, transformComponent.Position.y, transformComponent.Position.z }, { 0.0, 1.0, 0.0 }, transformComponent.Rotation, { 0.4f, 0.4f, 0.4f }, WHITE);
					else if(colorComponent.RabbitColor == Simulation::AnimalsColors::Black)
						DrawModelEx(*m_Models->GetBlackFoxModel(), { transformComponent.Position.x, transformComponent.Position.y, transformComponent.Position.z }, { 0.0, 1.0, 0.0 }, transformComponent.Rotation, { 0.4f, 0.4f, 0.4f }, WHITE);
					else if(colorComponent.RabbitColor == Simulation::AnimalsColors::Red)
						DrawModelEx(*m_Models->GetRedFoxModel(), { transformComponent.Position.x, transformComponent.Position.y, transformComponent.Position.z }, { 0.0, 1.0, 0.0 }, transformComponent.Rotation, { 0.4f, 0.4f, 0.4f }, WHITE);
				}
			}

			Processes();

			for (auto& i : m_Scene->GetEntities<Core::TransformComponent>())
			{
				UpdateEntityPosition(i);
			}

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

			ImGui::SameLine((GetScreenWidth() / 2) - 22.5);
			if (ImGui::Button("Run", { 45, 20 }))
				m_IsRunning = !m_IsRunning;

			ImGui::EndMainMenuBar();
		}

		bool oppened = false;

		ImGui::Begin("Viewport", &oppened, ImGuiWindowFlags_NoDecoration);

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
			Simulation::WorldLayer bufLayer;

			for (size_t i = 0; i < m_World->GetLayers()[m_World->GetLayers().size() - 1].Tiles.size(); i++)
			{
				for (size_t j = 0; j <  m_World->GetLayers()[ m_World->GetLayers().size() - 1].Tiles[i].size(); j++)
				{
					if ( m_World->GetLayers()[ m_World->GetLayers().size() - 1].Tiles[i][j].Type != Simulation::TileType::None)
					{
						m_World->GetLayers().push_back(bufLayer);
						break;
					}
				}
			}

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

					uint8_t color = GetRandomValue(0, 100);
					if (color <= 55)
						color = 1;
					else if (color > 55 && color <= 80)
						color = 2;
					else
						color = 3;

					Core::Entity rabbit(m_Scene);
					rabbit.AddComponent<Core::TagComponent>("Rabbit");
					rabbit.AddComponent<Core::TransformComponent>(glm::vec3(xPosition - 5, 0.5f + i, zPosition - 5), 0.0f);
					rabbit.AddComponent<Simulation::TileLocation>(xPosition, zPosition, i);
					rabbit.AddComponent<Simulation::MoveDirectionComponent>(1);
					rabbit.AddComponent<Simulation::AnimalComponent>(6000, 12000, Simulation::Animals::Rabbit);
					rabbit.AddComponent<Simulation::ColorComponent>((Simulation::AnimalsColors)color);
				}
			}
			m_RabbitsToSpawn = 0;


			for (int j = 0; j < m_FoxesToSpawn; j++)
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

					uint8_t color = GetRandomValue(0, 100);
					if (color <= 55)
						color = 1;
					else if (color > 55 && color <= 80)
						color = 3;
					else
						color =4;

					Core::Entity fox(m_Scene);
					fox.AddComponent<Core::TagComponent>("Fox");
					fox.AddComponent<Core::TransformComponent>(glm::vec3(xPosition - 5, 0.5f + i, zPosition - 5), 0.0f);
					fox.AddComponent<Simulation::TileLocation>(xPosition, zPosition, i);
					fox.AddComponent<Simulation::MoveDirectionComponent>(1);
					fox.AddComponent<Simulation::AnimalComponent>(4500, 6000, Simulation::Animals::Fox);
					fox.AddComponent<Simulation::ColorComponent>((Simulation::AnimalsColors)color);
				}
			}
			m_FoxesToSpawn = 0;
		}

		ImGui::End();

		ImGui::Begin("Scene Elements", &oppened, ImGuiWindowFlags_NoDecoration);

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

		ImGui::Begin("Config", &oppened, ImGuiWindowFlags_NoDecoration);

		if (!m_IsWorldEditEnabled)
		{
			if (m_Scene->GetSelectedEntity() != entt::null)
			{
				Core::TagComponent& tag = m_Scene->GetComponent<Core::TagComponent>(m_Scene->GetSelectedEntity());
				ImGui::Text(((std::string)"Tag: " + tag.Tag).c_str());

				ImGui::Spacing();

				Core::TransformComponent& transform = m_Scene->GetComponent<Core::TransformComponent>(m_Scene->GetSelectedEntity());
				ImGui::DragFloat3("Position", glm::value_ptr(transform.Position), 0.2f, -5.0f, 4.0f);

				Simulation::MoveDirectionComponent& dir = m_Scene->GetComponent<Simulation::MoveDirectionComponent>(m_Scene->GetSelectedEntity());
				ImGui::Text(((std::string)"Move Dir: " + std::to_string(dir.MoveDir)).c_str());

				Simulation::AnimalComponent& animal = m_Scene->GetComponent<Simulation::AnimalComponent>(m_Scene->GetSelectedEntity());
				ImGui::Text(((std::string)"Hunger: " + std::to_string(animal.Hunger)).c_str());

				ImGui::Text(((std::string)"Thirst: " + std::to_string(animal.Thirst)).c_str());

				ImGui::Text(((std::string)"Reproductive Urge: " + std::to_string(animal.ReplroductiveUrges)).c_str());

				Simulation::ColorComponent& color = m_Scene->GetComponent<Simulation::ColorComponent>(m_Scene->GetSelectedEntity());
				ImGui::Text(((std::string)"Color: " + color.ToString()).c_str());

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
			ImGui::Begin("Layers", &oppened, ImGuiWindowFlags_NoDecoration);

			uint8_t index = 0;
			for (auto& layer : m_World->GetLayers())
			{
				ImGui::PushID(index);

				if (ImGui::Selectable(((std::string)"Layer" + std::to_string(index)).c_str(), m_SelectedLayer == index))
					m_SelectedLayer = index;

				if (ImGui::Button("Remove Layer"))
					m_World->PopLayer(index); 

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
			ImGui::Begin("Animals", &oppened, ImGuiWindowFlags_NoDecoration);

			ImGui::Spacing();
			ImGui::DragInt("Rabbits", &m_RabbitsToSpawn, 1.0, 0, 100);
			ImGui::Spacing();
			ImGui::DragInt("Foxes", &m_FoxesToSpawn, 1.0, 0, 100);
			ImGui::Spacing();

			ImGui::End();	
		}
	}

	void EditorLayer::UpdateEntityPosition(entt::entity entity)
	{
		Core::TransformComponent& transformComponent = m_Scene->GetComponent<Core::TransformComponent>(entity);
		Simulation::MoveDirectionComponent& moveDirComp = m_Scene->GetComponent<Simulation::MoveDirectionComponent>(entity);
		Simulation::AnimalComponent& animal = m_Scene->GetComponent<Simulation::AnimalComponent>(entity);
				
		uint8_t changeDir = GetRandomValue(1, 6);
		uint8_t priorityDir = GetRandomValue(1, 2);

		if (m_Wait == 0)
		{
			m_Wait = 100;

			if (animal.Hunger > 3000)
			{
				if (changeDir == 1 && moveDirComp.CanChange)
					moveDirComp.MoveDir = GetRandomValue(1, 8);
			}
			else
			{
				UpdateEntityPositionOnHunger(entity);
			}

			if (animal.Thirst > 2025)
			{
				if (changeDir == 1 && moveDirComp.CanChange)
					moveDirComp.MoveDir = GetRandomValue(1, 8);
			}
			else
			{
				UpdateEntityPositionOnThirst(entity);
			}

			if (animal.ReplroductiveUrges < 1500)
			{
				if (changeDir == 1 && moveDirComp.CanChange)
					moveDirComp.MoveDir = GetRandomValue(1, 8);	
			}
			else
			{
				UpdateEntityPositionOnReproduction(entity);
			}

			if (transformComponent.Position.x > -5.0 && transformComponent.Position.x < 4.0 && transformComponent.Position.z > -5.0 && transformComponent.Position.z < 4.0)
			{
				moveDirComp.CanChange = true;

				if (moveDirComp.MoveDir == 1)
				{
					transformComponent.Position.x += 0.2;
					transformComponent.Rotation = 90.0;
				}
				else if (moveDirComp.MoveDir == 2)
				{
					transformComponent.Position.x -= 0.2;
					transformComponent.Rotation = -90.0f;
				}
				else if (moveDirComp.MoveDir == 3)
				{
					transformComponent.Position.z += 0.2;
					transformComponent.Rotation = 0.0f;
				}
				else if (moveDirComp.MoveDir == 4)
				{
					transformComponent.Position.z -= 0.2;
					transformComponent.Rotation = -180.0f;
				}
				else if (moveDirComp.MoveDir == 5)
				{
					transformComponent.Position.z += 0.2;
					transformComponent.Position.x += 0.2;
					transformComponent.Rotation = 45.0f;
				}
				else if (moveDirComp.MoveDir == 6)
				{
					transformComponent.Position.z -= 0.2;
					transformComponent.Position.x += 0.2;
					transformComponent.Rotation = 135.0f;
				}
				else if (moveDirComp.MoveDir == 7)
				{
					transformComponent.Position.z -= 0.2;
					transformComponent.Position.x -= 0.2;
					transformComponent.Rotation = -135.0f;
				}
				else if (moveDirComp.MoveDir == 8)
				{
					transformComponent.Position.z += 0.2;
					transformComponent.Position.x -= 0.2;
					transformComponent.Rotation = -45.0f;
				}
			}	
			else
			{
				uint32_t oldDir = moveDirComp.MoveDir;
				while (moveDirComp.MoveDir == oldDir)
					moveDirComp.MoveDir = GetRandomValue(1, 8);

				moveDirComp.CanChange = false;
				
				if (transformComponent.Position.x < 0)
					transformComponent.Position.x += 0.2f;
				else 
					transformComponent.Position.x -= 0.2f;

				if (transformComponent.Position.z < 0)
					transformComponent.Position.z += 0.2f;
				else
					transformComponent.Position.z -= 0.2f;
			}
		}
		else
		{
			m_Wait--;
		}
	}

	void EditorLayer::UpdateEntityPositionOnHunger(entt::entity entity)
	{
		Core::TransformComponent& transformComponent = m_Scene->GetComponent<Core::TransformComponent>(entity);
		Simulation::AnimalComponent& animal = m_Scene->GetComponent<Simulation::AnimalComponent>(entity);

		uint32_t entityTileX = transformComponent.Position.z + 5;
		uint32_t entityTileZ = transformComponent.Position.x + 5;

		int32_t closestGrassTileZ = 0;
		int32_t closestGrassTileX = 0;

		int32_t moveX = 0;
		int32_t moveZ = 0;

		entt::entity prey = entt::null;
		Core::TransformComponent transformComponentSecondEntity = m_Scene->GetComponent<Core::TransformComponent>(entity);

		if (animal.Type == Simulation::Animals::Rabbit)
		{
			for (uint32_t i = 0; i < m_World->GetLayers().size(); i++)
			{
				for (uint32_t j = 0; j < m_World->GetLayers()[i].Tiles.size(); j++)
				{
					for (uint32_t k = 0; k < m_World->GetLayers()[i].Tiles[j].size(); k++)
					{
						if (m_World->GetLayers()[i].Tiles[j][k].Type == Simulation::TileType::Grass)
						{
							if (closestGrassTileX < transformComponent.Position.x + 5)
								closestGrassTileX = transformComponent.Position.x + 5;

							if (closestGrassTileZ < transformComponent.Position.z + 5)
								closestGrassTileZ = transformComponent.Position.z + 5;
						}
					}
				}
			}

			moveX = (closestGrassTileX - 5 > transformComponent.Position.x) ? 0.2 : -0.2;
			moveZ = (closestGrassTileZ - 5 > transformComponent.Position.z) ? 0.2 : -0.2;

		}	
		else if (animal.Type == Simulation::Animals::Fox)
		{
			for (auto& i : m_Scene->GetEntities<Core::TransformComponent>())
			{
				Simulation::AnimalComponent& animalPrey = m_Scene->GetComponent<Simulation::AnimalComponent>(i);

				if (animalPrey.Type == Simulation::Animals::Rabbit)
				{
					transformComponentSecondEntity = m_Scene->GetComponent<Core::TransformComponent>(i);
					prey = i;
					break;
				}
			}

			moveX = (transformComponentSecondEntity.Position.x > transformComponent.Position.x) ? 0.2 : -0.2;
			moveZ = (transformComponentSecondEntity.Position.z > transformComponent.Position.z) ? 0.2 : -0.2;
		}

		transformComponent.Position.z += moveZ;
		transformComponent.Position.x += moveX;

		if (CheckCollisionRecs({transformComponent.Position.x, transformComponent.Position.z, 2.5f, 2.5f}, {transformComponentSecondEntity.Position.x, transformComponentSecondEntity.Position.z, 5.0, 5.0}) && prey != entt::null)
		{
			m_Scene->SetSelectedEntity(entt::null);
			m_Scene->Destroy(prey);
			animal.Hunger = 6000;
		}
	}

	void EditorLayer::UpdateEntityPositionOnThirst(entt::entity entity)
	{
		Core::TransformComponent& transformComponent = m_Scene->GetComponent<Core::TransformComponent>(entity);
		Simulation::AnimalComponent& animal = m_Scene->GetComponent<Simulation::AnimalComponent>(entity);
		Simulation::MoveDirectionComponent& moveDirComp = m_Scene->GetComponent<Simulation::MoveDirectionComponent>(entity);
		uint32_t entityTileX = transformComponent.Position.z + 5;
		uint32_t entityTileZ = transformComponent.Position.x + 5;

		int32_t closestWaterTileZ = 0;
		int32_t closestWaterTileX = 0;

		if (animal.Type == Simulation::Animals::Rabbit)
		{
			for (uint32_t i = 0; i < m_World->GetLayers().size(); i++)
			{
				for (uint32_t j = 0; j < m_World->GetLayers()[i].Tiles.size(); j++)
				{
					for (uint32_t k = 0; k < m_World->GetLayers()[i].Tiles[j].size(); k++)
					{
						if (m_World->GetLayers()[i].Tiles[j][k].Type == Simulation::TileType::Grass)
						{
							if (closestWaterTileZ < transformComponent.Position.x + 5)
								closestWaterTileZ = transformComponent.Position.x + 5;

							if (closestWaterTileX < transformComponent.Position.z + 5)
								closestWaterTileX = transformComponent.Position.z + 5;
						}
					}
				}
			}

			int32_t moveX = (closestWaterTileX - 5 > transformComponent.Position.x) ? 0.2 : -0.2;
			int32_t moveZ = (closestWaterTileZ - 5 > transformComponent.Position.z) ? 0.2 : -0.2;

			transformComponent.Position.x += moveX;
			transformComponent.Position.z += moveZ;
		}
	}

	void EditorLayer::UpdateEntityPositionOnReproduction(entt::entity entity)
	{
		Core::TransformComponent& transformComponent = m_Scene->GetComponent<Core::TransformComponent>(entity);
		Simulation::AnimalComponent& animal = m_Scene->GetComponent<Simulation::AnimalComponent>(entity);
		Simulation::ColorComponent colorComponetOne = m_Scene->GetComponent<Simulation::ColorComponent>(entity);

		entt::entity newEntity = entt::null;


		if (animal.Type == Simulation::Animals::Rabbit)
		{
			for (auto& i : m_Scene->GetEntities<Core::TransformComponent>())
			{
				if (animal.ReplroductiveUrges > 1500 && i != entity)
				{
					newEntity = i;
					animal.ReplroductiveUrges = 0;
					break;
				}
			}
			
			if (entity != entt::null)
			{
				Simulation::ColorComponent colorComponetTwo = m_Scene->GetComponent<Simulation::ColorComponent>(entity);
				Core::TransformComponent transformComponentSecondEntity = m_Scene->GetComponent<Core::TransformComponent>(entity);

				int32_t moveX = (transformComponentSecondEntity.Position.x > transformComponent.Position.x) ? 0.2 : -0.2;
				int32_t moveZ = (transformComponentSecondEntity.Position.z > transformComponent.Position.z) ? 0.2 : -0.2;

				transformComponent.Position.x += moveX;
				transformComponent.Position.z += moveZ;


				bool failuare = GetRandomValue(0, 1);

				if (CheckCollisionRecs({ transformComponent.Position.x, transformComponent.Position.z, 2.5, 2.5f }, { transformComponentSecondEntity.Position.x, transformComponentSecondEntity.Position.z, 5.0f, 5.0f }) && !failuare)
				{
					Core::Entity rabbit(m_Scene);
					rabbit.AddComponent<Core::TagComponent>("Rabbit");
					rabbit.AddComponent<Core::TransformComponent>(glm::vec3(transformComponent.Position.x, transformComponent.Position.y, transformComponent.Position.z), 0.0f);
					rabbit.AddComponent<Simulation::TileLocation>(transformComponent.Position.z + 5, transformComponent.Position.x + 5, transformComponent.Position.y);
					rabbit.AddComponent<Simulation::AnimalComponent>(4500, 6000, Simulation::Animals::Rabbit);
					rabbit.AddComponent<Simulation::MoveDirectionComponent>(1);

					uint32_t procentage = GetRandomValue(1, 100);
					
					if (colorComponetTwo.RabbitColor == Simulation::AnimalsColors::White && colorComponetOne.RabbitColor == Simulation::AnimalsColors::Gray)
					{
						if(procentage < 50)
							rabbit.AddComponent<Simulation::ColorComponent>(Simulation::AnimalsColors::Gray);			
						else
							rabbit.AddComponent<Simulation::ColorComponent>(Simulation::AnimalsColors::White);			
					}
					else if (colorComponetTwo.RabbitColor == Simulation::AnimalsColors::White && colorComponetOne.RabbitColor == Simulation::AnimalsColors::White)
					{
						rabbit.AddComponent<Simulation::ColorComponent>(Simulation::AnimalsColors::White);			
					}
					else if (colorComponetTwo.RabbitColor == Simulation::AnimalsColors::Black && colorComponetOne.RabbitColor == Simulation::AnimalsColors::Gray)
					{
						if(procentage < 50)
							rabbit.AddComponent<Simulation::ColorComponent>(Simulation::AnimalsColors::Gray);			
						else
							rabbit.AddComponent<Simulation::ColorComponent>(Simulation::AnimalsColors::Black);			
					}
					else if (colorComponetTwo.RabbitColor == Simulation::AnimalsColors::Black && colorComponetOne.RabbitColor == Simulation::AnimalsColors::White)
					{
						if(procentage < 70)
							rabbit.AddComponent<Simulation::ColorComponent>(Simulation::AnimalsColors::Gray);			
						else
							rabbit.AddComponent<Simulation::ColorComponent>(Simulation::AnimalsColors::White);			
					}
					else if (colorComponetTwo.RabbitColor == Simulation::AnimalsColors::Black && colorComponetOne.RabbitColor == Simulation::AnimalsColors::Black)
					{
						rabbit.AddComponent<Simulation::ColorComponent>(Simulation::AnimalsColors::Black);			
					}
					else if (colorComponetTwo.RabbitColor == Simulation::AnimalsColors::Gray && colorComponetOne.RabbitColor == Simulation::AnimalsColors::Gray)
					{
						if (procentage < 50)
							rabbit.AddComponent<Simulation::ColorComponent>(Simulation::AnimalsColors::Gray);
						else if (procentage < 75)
							rabbit.AddComponent<Simulation::ColorComponent>(Simulation::AnimalsColors::Black);
						else
							rabbit.AddComponent<Simulation::ColorComponent>(Simulation::AnimalsColors::White);
					}
					else
					{
						rabbit.AddComponent<Simulation::ColorComponent>(Simulation::AnimalsColors::Black);
					}
				}
			}
		}
		else if (animal.Type == Simulation::Animals::Fox)
		{
			for (auto& i : m_Scene->GetEntities<Core::TransformComponent>())
			{
				if (i != entity && animal.ReplroductiveUrges > 1500)
				{
					newEntity = i;
					animal.ReplroductiveUrges = 0;
					break;
				}
			}

			if (entity != entt::null)
			{
				Simulation::ColorComponent colorComponetTwo = m_Scene->GetComponent<Simulation::ColorComponent>(entity);
				Core::TransformComponent transformComponentSecondEntity = m_Scene->GetComponent<Core::TransformComponent>(entity);


				int32_t moveX = (transformComponentSecondEntity.Position.x > transformComponent.Position.x) ? 0.2 : -0.2;
				int32_t moveZ = (transformComponentSecondEntity.Position.z > transformComponent.Position.z) ? 0.2 : -0.2;

				transformComponent.Position.x += moveX;
				transformComponent.Position.z += moveZ;

				bool failuare = GetRandomValue(0, 1);

				if (CheckCollisionRecs({ transformComponent.Position.x, transformComponent.Position.z, 2.5, 2.5f }, { transformComponentSecondEntity.Position.x, transformComponentSecondEntity.Position.z, 5.0f, 5.0f }) && !failuare)
				{
					Core::Entity fox(m_Scene);
					fox.AddComponent<Core::TagComponent>("Fox");
					fox.AddComponent<Core::TransformComponent>(glm::vec3(transformComponent.Position.x, transformComponent.Position.y, transformComponent.Position.z), 0.0f);
					fox.AddComponent<Simulation::TileLocation>(transformComponent.Position.z + 5, transformComponent.Position.x + 5, transformComponent.Position.y);
					fox.AddComponent<Simulation::AnimalComponent>(4500, 6000, Simulation::Animals::Fox);
					fox.AddComponent<Simulation::MoveDirectionComponent>(1);

					uint32_t procentage = GetRandomValue(1, 100);

					if (colorComponetTwo.RabbitColor == Simulation::AnimalsColors::White && colorComponetOne.RabbitColor == Simulation::AnimalsColors::Red)
					{
						if (procentage < 50)
							fox.AddComponent<Simulation::ColorComponent>(Simulation::AnimalsColors::Red);
						else
							fox.AddComponent<Simulation::ColorComponent>(Simulation::AnimalsColors::White);
					}
					else if (colorComponetTwo.RabbitColor == Simulation::AnimalsColors::White && colorComponetOne.RabbitColor == Simulation::AnimalsColors::White)
					{
						fox.AddComponent<Simulation::ColorComponent>(Simulation::AnimalsColors::White);
					}
					else if (colorComponetTwo.RabbitColor == Simulation::AnimalsColors::Black && colorComponetOne.RabbitColor == Simulation::AnimalsColors::Red)
					{
						if (procentage < 50)
							fox.AddComponent<Simulation::ColorComponent>(Simulation::AnimalsColors::Red);
						else
							fox.AddComponent<Simulation::ColorComponent>(Simulation::AnimalsColors::Black);
					}
					else if (colorComponetTwo.RabbitColor == Simulation::AnimalsColors::Black && colorComponetOne.RabbitColor == Simulation::AnimalsColors::White)
					{
						if (procentage < 70)
							fox.AddComponent<Simulation::ColorComponent>(Simulation::AnimalsColors::Red);
						else
							fox.AddComponent<Simulation::ColorComponent>(Simulation::AnimalsColors::White);
					}
					else if (colorComponetTwo.RabbitColor == Simulation::AnimalsColors::Black && colorComponetOne.RabbitColor == Simulation::AnimalsColors::Black)
					{
						fox.AddComponent<Simulation::ColorComponent>(Simulation::AnimalsColors::Black);
					}
					else if (colorComponetTwo.RabbitColor == Simulation::AnimalsColors::Red && colorComponetOne.RabbitColor == Simulation::AnimalsColors::Red)
					{
						if (procentage < 50)
							fox.AddComponent<Simulation::ColorComponent>(Simulation::AnimalsColors::Red);
						else if (procentage < 75)
							fox.AddComponent<Simulation::ColorComponent>(Simulation::AnimalsColors::Black);
						else
							fox.AddComponent<Simulation::ColorComponent>(Simulation::AnimalsColors::White);
					}
					else
					{
						fox.AddComponent<Simulation::ColorComponent>(Simulation::AnimalsColors::Black);
					}
				}
			}
		}
	}

	void EditorLayer::Processes()
	{
		for (auto& i : m_Scene->GetEntities<Simulation::AnimalComponent>())
		{
			Core::TransformComponent& transformComponent = m_Scene->GetComponent<Core::TransformComponent>(i);
			Simulation::AnimalComponent& animal = m_Scene->GetComponent<Simulation::AnimalComponent>(i);

			if (animal.Hunger > 0)
			{
				animal.Hunger--;
			}
			else
			{
				m_Scene->SetSelectedEntity(entt::null);
				m_Scene->Destroy(i);
			}

			if (animal.Thirst > 0)
			{
				animal.Thirst--;
			}
			else
			{
				m_Scene->SetSelectedEntity(entt::null);
				m_Scene->Destroy(i);
			}

			if (animal.ReplroductiveUrges > 3000)
			{
				m_Scene->SetSelectedEntity(entt::null);
				m_Scene->Destroy(i);
			}
			else
			{
				if(animal.Hunger > 5000 && animal.Thirst > 3000)
					animal.ReplroductiveUrges++;
			}
		
			if (animal.Type == Simulation::Animals::Rabbit)
			{
				if (animal.Hunger < 3000)
				{
					if ((transformComponent.Position.z + 5 * 10) + transformComponent.Position.x + 5 > 10)
					{
						if (m_World->GetLayers()[transformComponent.Position.y].Tiles[transformComponent.Position.z + 5][transformComponent.Position.x + 5].Type == Simulation::TileType::Grass)
							animal.Hunger = 6000;
					}
				}
			}

			if (animal.Thirst < 2250)
			{
				if ((transformComponent.Position.z + 5 * 10) + transformComponent.Position.x + 5 > 10)
				{
					if (m_World->GetLayers()[transformComponent.Position.y].Tiles[transformComponent.Position.z + 5][transformComponent.Position.x + 5].Type == Simulation::TileType::Water)
						animal.Thirst = 4500;
				}
			}
		}	
	}
}