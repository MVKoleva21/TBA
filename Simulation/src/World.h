#pragma once
#include <vector>
#include <cstdint>
#include <string>
#include <fstream>
#include <array>

namespace Simulation {
	/// @brief The types of tiles
	enum class TileType 
	{
		None = 0, Sand, Water, Grass	
	};

	/// @brief The tile in the world
	struct WorldTile
	{
		float XPosition = 0;
		float YPositon = 0;
		TileType Type = TileType::None;
		bool IsThereEntity = false;
	};

	/// @brief A layer of the world
	struct WorldLayer
	{
		std::array<std::array<WorldTile, 10>, 10> Tiles;
	};

	/// @brief The world
	class World
	{
	public:
		/// @brief Construct a new World object
		World();
		~World() = default;

		/// @brief Get the tiles of a layer
		/// @param layer The layer to get the tiles from 
		/// @return The tiles of the layer 
		std::array<std::array<WorldTile, 10>, 10> GetTiles(uint32_t layer) { return m_WorldLayers[layer].Tiles; }

		/// @brief Get the world layers
		/// @return Get the world layers
		std::vector<WorldLayer>& GetLayers() { return m_WorldLayers; }

		/// @brief Load the world from a file
		/// @param path The path to the file
		void LoadWorld(std::string path);

		/// @brief Save the world to a file
		/// @param path The path to the file
		void SaveWorld(std::string path);

		/// @brief Push a layer to the world
		/// @param layer The layer to push
		void PushLayer(WorldLayer layer);

		/// @brief Push a tile to the world
		/// @param layer Layer to push the tile to
		/// @param indexX X index of the tile
		/// @param indexY Y index of the tile
		/// @param tile Tile to push
		void PushTile(uint32_t layer, uint32_t indexX, uint32_t indexY, WorldTile tile) { m_WorldLayers[layer].Tiles[indexX][indexY] = tile; }

		/// @brief Pop a layer from the world
		/// @param index Index of the layer to pop
		void PopLayer(uint32_t index) { m_WorldLayers.erase(m_WorldLayers.begin() + index); }
		
	private:
		std::vector<WorldLayer> m_WorldLayers;
	};
}
