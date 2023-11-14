#pragma once
#include "Core.h"
#include "World.h"
#include "Models.h"

#include <imgui.h>
#include <glm/glm.hpp>
#include <thread>

namespace Editor {
	/// @brief The editor layer
	/// @details This layer is used to edit the world
	/// @see Core::Layer
	class EditorLayer : public Core::Layer
	{
	public:
		/// @brief Run once when the layer is attached
		void OnAttach();

		/// @brief Run every frame
		void OnUpdate();

		/// @brief Run every frame when ImGui is rendered
		void OnImGuiRender();

	private:
		/// @brief Run the simulating processes
		void Processes();

		/// @brief Update the entity position
		/// @param entity entity to update
		void UpdateEntityPosition(entt::entity entity);

		/// @brief Update the entity position on hunger
		/// @param entity entity to update
		void UpdateEntityPositionOnHunger(entt::entity entity);

		/// @brief Update the entity position on thirst
		/// @param entity entity to update
		void UpdateEntityPositionOnThirst(entt::entity entity);

		/// @brief Update the entity position on reproduction
		/// @param entity entity to update
		void UpdateEntityPositionOnReproduction(entt::entity entity);

	private:
		std::shared_ptr<Core::FrameBuffer> m_FrameBuffer;
		glm::vec2 m_ViewPortSize = {GetScreenWidth(), GetScreenHeight()};
		std::shared_ptr<Core::Scene> m_Scene;
		
		std::shared_ptr<Camera3D> m_PerspectiveCamera;

		std::shared_ptr<Simulation::World> m_World;

		bool m_IsWorldEditEnabled = false;

		uint32_t m_SceneEntitiesSelectorWidth = 0;

		Simulation::TileType m_SelectedTileType = Simulation::TileType::None;
		uint32_t m_SelectedLayer = 0;

		int32_t m_RabbitsToSpawn = 0;
		int32_t m_FoxesToSpawn = 0;
		bool m_IsRunning = false;

		std::shared_ptr<Simulation::Models> m_Models = nullptr;

		uint32_t m_Wait = 100;
	};
}
