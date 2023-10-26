#pragma once
#include "pch.h"
#include <raylib.h>

namespace Core {
	class FrameBuffer
	{
	public:
		FrameBuffer() = default;
		~FrameBuffer() = default;

		void Init(uint32_t width, uint32_t height);
		void Bind();
		void UnBind();

		uint32_t const GetWidth() { return m_Width; }
		uint32_t const GetHeight() { return m_Height; }

		std::shared_ptr<RenderTexture2D> const GetTexture() { return m_Texture; }

	private:
		uint32_t m_Width;
		uint32_t m_Height;

		std::shared_ptr<RenderTexture2D> m_Texture;
	};
}
