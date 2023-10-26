#include "pch.h"
#include "Application.h"

namespace Core {
	Application::Application()
	{
		assert(!s_Instance, "Application already exists");

		s_Instance = this;
	}

	void Application::Init(glm::vec2 size, std::string title)
	{
		m_Window.reset(Window::CreateWindow(size.x, size.y, title));
		m_Window->Init();
		rlImGuiSetup(true);
	}

	void Application::Run()
	{
		for(auto layer : m_LayerStack)
		{ 
			layer->OnAttach();
		}

		while (m_IsRunning)
		{
			ShouldWindowClose();

			BeginDrawing();

			for(auto layer : m_LayerStack)
			{ 
				layer->OnUpdate();
			}

			rlImGuiBegin();

			for(auto layer : m_LayerStack)
			{ 
				layer->OnImGuiRender();
			}

			rlImGuiEnd();

			EndDrawing();
		}

		for(auto layer : m_LayerStack)
		{ 
			layer->OnDetach();
		}

		rlImGuiShutdown();
		CloseWindow();
	}

	void Application::ShouldWindowClose()
	{
		if (WindowShouldClose())
		{
			m_IsRunning = false;
		}
	}
}