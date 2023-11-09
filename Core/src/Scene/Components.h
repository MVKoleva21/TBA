#pragma once
#include "pch.h"
#include <glm/glm.hpp>

namespace Core {
	struct TagComponent
	{
		std::string Tag;

		TagComponent(std::string tag)
		{
			Tag = tag;
		}
	};

	struct TransformComponent
	{
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		float Rotation = 0.0f;

		TransformComponent(glm::vec3 position, float rotation)
		{
			Position = position;
			Rotation = rotation;
		}
	};
}
