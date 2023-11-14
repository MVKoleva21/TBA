#pragma once
#include "pch.h"
#include "Scene.h"

namespace Core {
	/// @brief An entity is a container of components	
	class Entity
	{
	public:
		/// @brief Create an entity
		/// @param scene The scene the entity belongs to 
		Entity(std::shared_ptr<Scene> scene);
		~Entity() = default;

		/// @brief Add a component to the entity
		/// @tparam T The type of the component
		/// @tparam Args The arguments to pass to the component constructor
		/// @param args The arguments to pass to the component constructor	
		template<typename T, typename... Args>
		void AddComponent(Args... args)
		{
			m_Scene->m_Registry.emplace<T>(m_Entity, args...);
		}

		/// @brief Get a component from the entity
		/// @tparam T The type of the component
		template<typename T>
		void RemoveComponent()
		{
			m_Scene->m_Registry.erase<T>(m_Entity);
		}

		/// @brief Get a native entitiy
		/// @return The native entity
		const entt::entity GetNativeEntity() const
		{
			return m_Entity;
		}

	private:
		std::shared_ptr<Scene> m_Scene;
		entt::entity m_Entity;
	};
}
