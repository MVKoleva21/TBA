#pragma once
#include "pch.h"
#include <entt.hpp>

namespace Core {
	class Scene
	{
	public:
		Scene(std::string name = "New Scene") {}
		~Scene() = default;

		template<typename T>
		auto GetEntities()
		{
			return m_Registry.view<T>();
		}

		void SetSelectedEntity(entt::entity entity)
		{
			m_SelectedEntity = entity;
		}

		template<typename T>
		bool HasComponent(entt::entity entity)
		{
			return m_Registry.try_get<T>(entity) != nullptr;
		}

		template<typename T>
		T& GetComponent(entt::entity entity)
		{
			return m_Registry.get<T>(entity);
		}

		entt::entity GetSelectedEntity()
		{
			return m_SelectedEntity;
		}

		std::string GetName() const { return m_SceneName; }
		void SetName(std::string name) { m_SceneName = name; }

	private:
		std::string m_SceneName;
		entt::registry m_Registry;
		entt::entity m_SelectedEntity = entt::null;

		friend class Entity;
	};
}
