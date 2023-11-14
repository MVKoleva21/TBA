#pragma once
#include "pch.h"
#include <entt.hpp>

namespace Core {
	/// @brief A scene is a container of entities and components
	class Scene
	{
	public:
		/// @brief Create a scene
		/// @param name The name of the scene
		Scene(std::string name = "New Scene") {}
		~Scene() = default;

		/// @brief Get the entity
		/// @tparam T The type of the component
		/// @return The created entity
		template<typename T>
		auto GetEntities()
		{
			return m_Registry.view<T>();
		}

		/// @brief Check if an entity has a component
		/// @tparam T The type of the component
		/// @param entity The entity to check
		/// @return True if the entity has the component, false otherwise
		template<typename T>
		bool HasComponent(entt::entity entity)
		{
			return m_Registry.try_get<T>(entity) != nullptr;
		}

		/// @brief Get a component from an entity
		/// @tparam T The type of the component
		/// @param entity The entity to get the component from
		/// @return The component 
		template<typename T>
		T& GetComponent(entt::entity entity)
		{
			return m_Registry.get<T>(entity);
		}

		/// @brief Get the selected entity
		/// @return The selected entity
		entt::entity GetSelectedEntity()
		{
			return m_SelectedEntity;
		}

		/// @brief Set the selected entity
		/// @param entity The entity to select
		void SetSelectedEntity(entt::entity entity)
		{
			m_SelectedEntity = entity;
		}

		/// @brief Destroy an entity
		/// @param entity The entity to destroy
		void Destroy(entt::entity entity)
		{
			m_Registry.destroy(entity);
		}

		/// @brief Get the name of the scene
		std::string GetName() const { return m_SceneName; }

		/// @brief Set the name of the scene
		/// @param name The name of the scene
		void SetName(std::string name) { m_SceneName = name; }

	private:
		std::string m_SceneName;
		entt::registry m_Registry;
		entt::entity m_SelectedEntity = entt::null;

		friend class Entity;
	};
}
