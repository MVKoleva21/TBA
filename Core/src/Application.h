#pragma once
#include "pch.h"
#include "Window.h"

#include <glm/glm.hpp>
#include <raylib.h>

namespace Core {
	class Application
	{
	public:
		Application();
		virtual ~Application() = default;

		void Init(glm::vec2 size, std::string title);
		void Run();

		static Application* Get() { return s_Instance; }

		std::shared_ptr<Window> const GetWindow() { return m_Window; }

	private:
		void ShouldWindowClose();

	private:
		inline static Application* s_Instance;
		std::shared_ptr<Window> m_Window;

		bool m_IsRunning = true;
	};
}
