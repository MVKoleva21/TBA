#pragma once
#include "pch.h"
#include <raylib.h>
#include <glm/glm.hpp>

namespace Core {
	class Window
	{
	public:
		Window(uint32_t width, uint32_t height, std::string title);
		~Window() = default;

		void Init();
		
		static Window* CreateWindow(uint32_t width, uint32_t height, std::string title) { return new Window(width, height, title); }
		
		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHight() const { return m_Height; }
		glm::vec2 GetSize() const { return {m_Width, m_Height}; }

	private:
		uint32_t m_Width;
		uint32_t m_Height;
		std::string m_Title;
	};
}
