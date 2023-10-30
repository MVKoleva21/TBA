#pragma once
#include "Core.h"
#include <imgui.h>
#include <glm/glm.hpp>

namespace Editor {
	class EditorLayer : public Core::Layer
	{
	public:
		void OnAttach();
		void OnUpdate();
		void OnImGuiRender();
	
	private:
		std::shared_ptr<Core::FrameBuffer> m_FrameBuffer;
		glm::vec2 m_ViewPortSize = {GetScreenWidth(), GetScreenHeight()};
		std::shared_ptr<Core::Scene> m_Scene;
		
		std::shared_ptr<Camera3D> m_PerspectiveCamera;

		std::shared_ptr<Core::World> m_World;

		bool m_IsWorldEditEnabled = false;

		uint32_t m_SceneEntitiesSelectorWidth = 0;

		Core::TileType m_SelectedTileType = Core::TileType::None;
		uint32_t m_SelectedLayer = 0;
	};
}
