#pragma once
#include "pch.h"
#include "Scene.h"

namespace Core {
	class Entity
	{
	public:
		Entity(std::shared_ptr<Scene> scene);
		~Entity() = default;

		template<typename T, typename... Args>
		void AddComponent(Args... args)
		{
			m_Scene->m_Registry.emplace<T>(m_Entity, args...);
		}

		template<typename T>
		void RemoveComponent()
		{
			m_Scene->m_Registry.erase<T>(m_Entity);
		}

		const entt::entity GetNativeEntity() const
		{
			return m_Entity;
		}

	private:
		std::shared_ptr<Scene> m_Scene;
		entt::entity m_Entity;
	};
}
