#pragma once
#include <vector>
#include <cstdint>
#include <string>
#include <fstream>
#include <array>

namespace Simulation {
	enum class TileType 
	{
		None, Sand, Water, Grass	
	};

	struct WorldTile
	{
		float XPosition = 0;
		float YPositon = 0;
		TileType Type = TileType::None;
	};

	struct WorldLayer
	{
		std::array<WorldTile, 100> Tiles;
	};

	class World
	{
	public:
		World();
		~World() = default;

		std::array<WorldTile, 100> GetTiles(uint32_t layer) { return m_WorldLayers[layer].Tiles; }
		std::vector<WorldLayer>& GetLayers() { return m_WorldLayers; }
		void LoadWorld(std::string path);
		void SaveWorld(std::string path);

		void PushLayer(WorldLayer layer) { m_WorldLayers.push_back(layer); }
		void PushTile(uint32_t layer, uint32_t index, WorldTile tile) { m_WorldLayers[layer].Tiles[index] = tile; }

		void PopLayer(uint32_t index) { m_WorldLayers.erase(m_WorldLayers.begin() + index - 1); }
		
	private:
		std::vector<WorldLayer> m_WorldLayers;
	};
}
