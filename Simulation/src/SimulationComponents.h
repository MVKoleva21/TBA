#pragma once
#include <cstdint>

namespace Simulation {
	/// @brief The colors of the animals
	enum class AnimalsColors : uint8_t
	{
		None = 0, Black, Gray, White, Red
	};

	/// @brief The types of animals
	enum class Animals : uint8_t
	{
		None = 0, Rabbit, Fox
	};

	/// @brief The location in the tilemap
	struct TileLocation
	{
		uint32_t ArrayPosX = 0;
		uint32_t ArrayPosY = 0;
		uint32_t Layer = 0;

		/// @brief Construct a new Tile Location object
		/// @param arrayPosX X position in the array 
		/// @param arrayPosY Y position in the array
		/// @param zIndex Z position in the array
		TileLocation(uint32_t arrayPosX, uint32_t arrayPosY, uint32_t zIndex)
		{
			ArrayPosX = arrayPosX;
			ArrayPosY = arrayPosY;
			Layer = zIndex;
		}
	};

	/// @brief The color component of the animals
	struct ColorComponent
	{
		AnimalsColors RabbitColor = AnimalsColors::None;

		/// @brief Construct a Color Component object
		/// @param color The color of the animal 
		ColorComponent(AnimalsColors color)
		{
			RabbitColor = color;
		}

		/// @brief Convert the color to a string
		/// @return The color as a string 
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
			case AnimalsColors::Red:
				return "Red";
			default:
				break;
			}
		}
	};

	/// @brief The move direction component of the animals
	struct MoveDirectionComponent
	{
		uint32_t MoveDir = 1;
		bool CanChange = true;

		/// @brief Construct a Move Direction Component object
		/// @param dir The direction of the animal
		MoveDirectionComponent(uint32_t dir)
		{
			MoveDir = dir;
		}
	};

	/// @brief The hunger, thirst and reproductiveUrge component of the animals
	struct AnimalComponent
	{
		uint32_t Thirst = 0;
		uint32_t Hunger = 0;
		Animals Type = Animals::None;
		uint32_t ReplroductiveUrges = 0;

		/// @brief Construct a Animal Component object
		/// @param thirst The thirst value of the animal
		/// @param hunger The hunger value of the animal 
		/// @param type The type of the animal 
		AnimalComponent(uint32_t thirst, uint32_t hunger, Animals type)
		{
			Thirst = thirst;
			Hunger = hunger;
			Type = type;
		}
	};
}
