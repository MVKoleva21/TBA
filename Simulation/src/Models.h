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


		Model* GetArcticFoxModel() { return &m_ArcticFoxModel; }
		Model* GetBlackFoxModel() { return &m_BlackFoxModel; }
		Model* GetRedFoxModel() { return &m_RedFoxModel; }
	private:
		Model m_GrayRabbitModel;
		Model m_BlackRabbitModel;
		Model m_WhiteRabbitModel;

		Model m_ArcticFoxModel;
		Model m_BlackFoxModel;
		Model m_RedFoxModel;
	};
}
