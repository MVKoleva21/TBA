#pragma once
#include "pch.h"
#include <raylib.h>

namespace Core {
	/// @brief Create a framebuffer to render to
	class FrameBuffer
	{
	public:
		FrameBuffer() = default;
		~FrameBuffer() = default;

		/// @brief Initialize the framebuffer
		/// @param width Width of the framebuffer
		/// @param height Height of the framebuffer
		void Init(uint32_t width, uint32_t height);

		/// @brief Bind the framebuffer
		void Bind();

		/// @brief Unbind the framebuffer
		void UnBind();

		/// @brief Get the width of the framebuffer
		/// @return The width of the framebuffer
		uint32_t const GetWidth() { return m_Width; }

		/// @brief Get the height of the framebuffer
		/// @return The height of the framebuffer
		uint32_t const GetHeight() { return m_Height; }

		/// @brief Get the texture of the framebuffer
		/// @return The texture of the framebuffer 
		std::shared_ptr<RenderTexture2D> const GetTexture() { return m_Texture; }

	private:
		uint32_t m_Width;
		uint32_t m_Height;

		std::shared_ptr<RenderTexture2D> m_Texture;
	};
}
