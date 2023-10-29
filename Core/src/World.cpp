#include "pch.h"
#include "World.h"

#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>

namespace Core {

	void World::LoadWorld(std::string path) 
	{
		YAML::Node worldFile = YAML::LoadFile(path);
		auto tiles = worldFile["Tiles"];

		for (auto tile : tiles)
		{
			WorldTile newTile;
			newTile.Type = (TileType)tile["TileType"].as<uint32_t>();
			newTile.XPosition = tile["PositionX"].as<float>();
			newTile.YPositon = tile["PositionY"].as<float>();

			uint32_t id = tile["Id"].as<uint32_t>();
			m_Tiles[id] = newTile;
		}
	}

	void World::SaveWorld(std::string path)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Tiles" << YAML::Value << YAML::BeginSeq;

		for (size_t i = 0; i < m_Tiles.size(); i++)
		{
			out << YAML::BeginMap;
			out << YAML::Key << "Id" << YAML::Value << i;
			out << YAML::Key << "TileType" << YAML::Value << (uint32_t)m_Tiles[i].Type;
			out << YAML::Key << "PositionX" << YAML::Value << m_Tiles[i].XPosition;
			out << YAML::Key << "PositionY" << YAML::Value << m_Tiles[i].YPositon;
			out << YAML::EndMap;
		}

		out << YAML::EndMap;
		out << YAML::EndSeq;

		std::ofstream fout(path);
		fout << out.c_str();
	}
}
