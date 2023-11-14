#pragma once
#include "pch.h"
#include "Window.h"
#include "Layer.h"

#include <glm/glm.hpp>
#include <raylib.h>
#include <rlImGui.h>
#include <imgui.h>

namespace Core {
	/// @brief The application
	class Application
	{
	public:
		/// @brief Create an application
		Application();
		virtual ~Application() = default;

		/// @brief Initialize the application
		/// @param size Window size
		/// @param title Window title
		void Init(glm::vec2 size, std::string title);

		/// @brief Run the application
		void Run();

		/// @brief Get the application
		/// @return The application
		static Application* Get() { return s_Instance; }

		/// @brief Get the window
		/// @return The window
		std::shared_ptr<Window> const GetWindow() { return m_Window; }

		/// @brief Push a layer to the layer stack
		/// @param layer The layer to push
		void PushLayer(std::shared_ptr<Layer> layer) { m_LayerStack.push_back(layer); }

	private:
		/// @brief Update the application
		void ShouldWindowClose();

	private:
		inline static Application* s_Instance;
		std::shared_ptr<Window> m_Window;

		bool m_IsRunning = true;

		std::vector<std::shared_ptr<Layer>> m_LayerStack;
	};
}
