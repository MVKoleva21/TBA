#pragma once
#include <cstdint>

namespace Simulation {
	enum class AnimalsColors
	{
		None = 0, Black, Gray, White
	};

	enum class Animals
	{
		None = 0, Rabbit, Fox
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

		std::string ToString()
		{
			switch (RabbitColor)
			{
			case AnimalsColors::Black:
				return "Black";
			case AnimalsColors::Gray:
				return "Gray";
			case AnimalsColors::White:
				return "White";
			default:
				break;
			}
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

	struct AnimalComponent
	{
		uint32_t Thirst = 0;
		uint32_t Hunger = 0;
		Animals Type = Animals::None;
		uint32_t ReplroductiveUrges = 0;

		AnimalComponent(uint32_t thirst, uint32_t hunger, Animals type)
		{
			Thirst = thirst;
			Hunger = hunger;
			Type = type;
		}
	};
}
