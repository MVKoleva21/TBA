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
	};
}
