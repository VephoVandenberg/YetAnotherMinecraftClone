#pragma once

#include "glm/glm.hpp"

namespace Engine
{
	class Ray
	{
	public:
		Ray(glm::vec3 pos, glm::vec3 direction, float magnitude);

		inline constexpr glm::vec3 getDirection() const { return m_direction; }
		inline constexpr glm::vec3 getPosition() const { return m_pos; }
		inline constexpr glm::vec3 getEndPoint() const { return m_endPoint; }
		inline constexpr float getLength() const { return m_length; }

		~Ray() = default;
		Ray(Ray&&) = default;
		Ray& operator=(Ray&&) = default;
		
		Ray() = delete;
		Ray(const Ray&) = delete;
		Ray& operator=(const Ray&) = delete;

	private:
		glm::vec3 m_pos;
		glm::vec3 m_direction;
		glm::vec3 m_endPoint;
		float m_length;
	};
}
