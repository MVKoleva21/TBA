#include "Models.h"
#include <iostream>

namespace Simulation {
	Models::Models()
	{
		m_GrayRabbitModel = LoadModel("../Simulation/assets/models/gray-rabbit.obj");
		m_BlackRabbitModel = LoadModel("../Simulation/assets/models/black-rabbit.obj");
		m_WhiteRabbitModel = LoadModel("../Simulation/assets/models/white-rabbit.obj");
	}
}