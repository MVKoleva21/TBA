#include "pch.h"
#include "World.h"

#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>

namespace Core {
	World::World()
	{
		WorldLayer layerZero;
		m_WorldLayers.push_back(layerZero);
	}

	void World::LoadWorld(std::string path) 
	{
		YAML::Node worldFile = YAML::LoadFile(path);
		auto layers = worldFile["Layers"];

		for (auto layer : layers)
		{
			WorldLayer newLayer;
			for (auto tile : layer["Layer"])
			{
				WorldTile newTile;
				newTile.Type = (TileType)tile["TileType"].as<uint32_t>();
				newTile.XPosition = tile["PositionX"].as<float>();
				newTile.YPositon = tile["PositionY"].as<float>();

				newLayer.Tiles[tile["Id"].as<uint32_t>()] = newTile;
			}

			m_WorldLayers.push_back(newLayer);
		}
	}

	void World::SaveWorld(std::string path)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Layers" << YAML::Value << YAML::BeginSeq;

		for (size_t i = 0; i < m_WorldLayers.size(); i++)
		{
			out << YAML::BeginMap;
			out << YAML::Key << "Layer" << YAML::Value << YAML::BeginSeq;
			for (size_t j = 0; j < m_WorldLayers[i].Tiles.size(); j++)
			{
				out << YAML::BeginMap;
				out << YAML::Key << "Id" << YAML::Value << j;
				out << YAML::Key << "TileType" << YAML::Value << (uint32_t)m_WorldLayers[i].Tiles[j].Type;
				out << YAML::Key << "PositionX" << YAML::Value << m_WorldLayers[i].Tiles[j].XPosition;
				out << YAML::Key << "PositionY" << YAML::Value << m_WorldLayers[i].Tiles[j].YPositon;
				out << YAML::EndMap;
			}	
			out << YAML::EndSeq;
			out << YAML::EndMap;
		}

		out << YAML::EndMap;
		out << YAML::EndSeq;

		std::ofstream fout(path);
		fout << out.c_str();
	}
}
