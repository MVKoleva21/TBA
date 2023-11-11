#pragma once
#include <cstdint>

namespace Simulation {
	enum class AnimalsColors
	{
		None, Gray, Black, White
	};

	struct TileLocation
	{
		uint32_t ArrayPosX = 0;
		uint32_t ArrayPosY = 0;
		uint32_t Layer = 0;

		TileLocation(uint32_t arrayPosX, uint32_t arrayPosY, uint32_t zIndex)
		{
			ArrayPosX = arrayPosX;
			ArrayPosY = arrayPosY;
			Layer = zIndex;
		}
	};

	struct ColorComponent
	{
		AnimalsColors RabbitColor = AnimalsColors::None;

		ColorComponent(AnimalsColors color)
		{
			RabbitColor = color;
		}
	};

	struct MoveDirectionComponent
	{
		uint32_t MoveDir = 1;
		bool CanChange = true;

		MoveDirectionComponent(uint32_t dir)
		{
			MoveDir = dir;
		}
	};
}
