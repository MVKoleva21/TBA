#pragma once
#include "pch.h"

namespace Core {

	class Scene
	{
	public:
		Scene(std::string name = "New Scene") {}
		~Scene() = default;

		std::string GetName() const { return m_SceneName; }
		void SetName(std::string name) { m_SceneName = name; }

	private:
		std::string m_SceneName;
	};
}
