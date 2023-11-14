#pragma once
#include <raylib.h>
#include <memory>

namespace Simulation {
	/// @brief Class to load models	
	class Models
	{
	public:
		/// @brief Load the models
		Models();
		~Models() = default;

		/// @brief Get Gray rabbit model
		/// @return The model
		Model* GetGrayRabbitModel() { return &m_GrayRabbitModel; }

		/// @brief Get Black rabbit model
		/// @return The model
		Model* GetBlackRabbitModel() { return &m_BlackRabbitModel; }

		/// @brief Get White rabbit model
		/// @return The model
		Model* GetWhiteRabbitModel() { return &m_WhiteRabbitModel; }

		/// @brief Get Arctic fox model
		/// @return The model
		Model* GetArcticFoxModel() { return &m_ArcticFoxModel; }

		/// @brief Get Black fox model
		/// @return The model
		Model* GetBlackFoxModel() { return &m_BlackFoxModel; }

		/// @brief Get Red fox model
		/// @return The model
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
