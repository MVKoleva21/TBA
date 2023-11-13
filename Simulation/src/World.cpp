#include "World.h"

#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>

namespace Simulation {
	World::World()
	{
		WorldLayer layerZero;
		m_WorldLayers.push_back(layerZero);
	}

	void World::LoadWorld(std::string path) 
	{
		m_WorldLayers.erase(m_WorldLayers.begin());
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

				newLayer.Tiles[tile["ArrPosX"].as<uint32_t>()][tile["ArrPosY"].as<uint32_t>()] = newTile;
			}

			m_WorldLayers.push_back(newLayer);
		}
	}

	void World::PushLayer(WorldLayer layer) 
	{ 
		m_WorldLayers.push_back(layer); 	
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
				for (size_t k = 0; k < m_WorldLayers[i].Tiles[j].size(); k++)
				{
					out << YAML::BeginMap;
					out << YAML::Key << "ArrPosX" << YAML::Value << j;
					out << YAML::Key << "ArrPosY" << YAML::Value << k;
					out << YAML::Key << "TileType" << YAML::Value << (uint32_t)m_WorldLayers[i].Tiles[j][k].Type;
					out << YAML::Key << "PositionX" << YAML::Value << m_WorldLayers[i].Tiles[j][k].XPosition;
					out << YAML::Key << "PositionY" << YAML::Value << m_WorldLayers[i].Tiles[j][k].YPositon;
					out << YAML::EndMap;
				}	
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
