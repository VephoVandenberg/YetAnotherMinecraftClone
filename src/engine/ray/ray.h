#pragma once

#include "glm/glm.hpp"

namespace Engine
{
	class Ray
	{
	public:
		Ray(glm::vec3 pos, glm::vec3 direction, float magnitude);

		Ray() = delete;
		~Ray() = default;

		Ray(const Ray&) = delete;
		Ray& operator=(const Ray&) = delete;
		Ray(Ray&&) = delete;
		Ray& operator=(Ray&&) = delete;

	private:
		glm::vec3 m_pos;
		glm::vec3 m_direction;
	};
}
