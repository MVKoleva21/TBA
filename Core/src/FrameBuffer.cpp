#include "pch.h"
#include "FrameBuffer.h"

namespace Core {
	void FrameBuffer::Init(uint32_t width, uint32_t height)
	{
		m_Width = width;
		m_Height = height;

		m_Texture.reset(new RenderTexture2D);
		*m_Texture = LoadRenderTexture(width, height);
	}

	void FrameBuffer::Bind()
	{
		BeginTextureMode(*m_Texture);
	}

	void FrameBuffer::UnBind()
	{
		EndTextureMode();
	}
}