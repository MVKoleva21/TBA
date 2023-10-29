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

		std::vector<WorldTile> GetTiles() { return m_Tiles; }
		void LoadWorld(std::string path) {}
		
	private:
		std::vector<WorldTile> m_Tiles;
	};
}
