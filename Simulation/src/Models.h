#pragma once
#include <raylib.h>
#include <memory>

namespace Simulation {
	
	class Models
	{
	public:
		Models();
		~Models() = default;

		Model* GetGrayRabbitModel() { return &m_GrayRabbitModel; }
		Model* GetBlackRabbitModel() { return &m_BlackRabbitModel; }
		Model* GetWhiteRabbitModel() { return &m_WhiteRabbitModel; }

	private:
		Model m_GrayRabbitModel;
		Model m_BlackRabbitModel;
		Model m_WhiteRabbitModel;
	};
}
