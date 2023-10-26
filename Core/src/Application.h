#pragma once
#include <glm/glm.hpp>
#include <raylib.h>

namespace Core {
	class Application
	{
	public:
		Application();
		virtual ~Application() = default;

		void Init(glm::vec2 size, const char* title);
		void Run();

		static Application* Get() { return s_Instance; }

		glm::vec2 GetSize() { return m_Size; }

	private:
		void ShouldWindowClose();

	private:
		inline static Application* s_Instance;
		glm::vec2 m_Size;

		bool m_IsRunning = true;
	};
}
