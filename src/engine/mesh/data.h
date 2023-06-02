#pragma once

#include <glm/glm.hpp>

namespace Engine::Data
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 textureCoords;
		glm::vec3 normal;
		float brightness;
	};
}