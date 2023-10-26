#include "pch.h"
#include "Application.h"

namespace Core {
	Application::Application()
	{
		assert(!s_Instance, "Application already exists");

		s_Instance = this;
	}

	void Application::Init(glm::vec2 size, const char* title)
	{
		m_Size = size;
		InitWindow(size.x, size.y, title);
	}

	void Application::Run()
	{
		while (m_IsRunning)
		{
			ShouldWindowClose();

			BeginDrawing();

			ClearBackground(RAYWHITE);

			DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

			EndDrawing();
		}
	}

	void Application::ShouldWindowClose()
	{
		if (WindowShouldClose())
		{
			m_IsRunning = false;
		}
	}
}