#pragma once
#include "pch.h"

namespace Core {
	enum class TileType 
	{
		None, Sand, Water, Grass	
	};

	struct WorldTile
	{
		float XPosition;
		float YPositon;
		TileType Type = TileType::None;
	};

	class World
	{
	public:
		World() = default;
		~World() = default;

		std::array<WorldTile, 100> GetTiles() { return m_Tiles; }
		void LoadWorld(std::string path) {}

		void PushTile(int index, WorldTile tile) { m_Tiles[index] = tile; }
		
	private:
		std::array<WorldTile, 100> m_Tiles;
	};
}
