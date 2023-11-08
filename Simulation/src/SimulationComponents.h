#pragma once
#include <cstdint>

namespace Simulation {
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
}
