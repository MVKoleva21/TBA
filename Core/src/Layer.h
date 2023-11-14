#pragma once
#include "pch.h"

namespace Core {
	/// @brief A layer in the application
	class Layer
	{
	public:
		Layer() = default;
		virtual ~Layer() = default;

		/// @brief Called when the layer is attached
		virtual void OnAttach() {}

		/// @brief Called when the layer is detached
		virtual void OnDetach() {}

		/// @brief Called when the layer is updated
		virtual void OnUpdate() {}

		/// @brief Called when ImGui is rendered
		virtual void OnImGuiRender() {}
	};
}
