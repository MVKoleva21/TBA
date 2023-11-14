#pragma once
#include "pch.h"
#include <raylib.h>
#include <glm/glm.hpp>

namespace Core {
	/// @brief A window to render to
	class Window
	{
	public:
		/// @brief Create a window
		/// @param width Width of the window
		/// @param height Height of the window
		/// @param title Title of the window
		Window(uint32_t width, uint32_t height, std::string title);
		~Window() = default;

		/// @brief Initialize the window
		void Init();
		
		/// @brief Update the window
		/// @param width Width of the window
		/// @param height Height of the window
		/// @param title Title of the window
		/// @return Window instance
		static Window* CreateWindow(uint32_t width, uint32_t height, std::string title) { return new Window(width, height, title); }
		
		/// @brief Get the width of the window
		/// @return The width of the window
		uint32_t GetWidth() const { return m_Width; }

		/// @brief Get the height of the window
		/// @return The height of the window
		uint32_t GetHight() const { return m_Height; }

		/// @brief Get the size of the window
		/// @return The size of the window as a glm::vec2
		glm::vec2 GetSize() const { return {m_Width, m_Height}; }

	private:
		uint32_t m_Width;
		uint32_t m_Height;
		std::string m_Title;
	};
}
