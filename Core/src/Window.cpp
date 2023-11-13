#include "pch.h"
#include "Window.h"

namespace Core {
	Window::Window(uint32_t width, uint32_t height, std::string title)
	{
		m_Width = width;
		m_Height = height;
		m_Title = title;
	}

	void Window::Init()
	{
		SetConfigFlags(FLAG_MSAA_4X_HINT);
		InitWindow(m_Width, m_Height, m_Title.c_str());
	}
}