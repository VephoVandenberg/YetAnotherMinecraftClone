#pragma once

#include <glm/glm.hpp>

namespace Engine
{
	class Camera
	{
	public:
		Camera();

		void update();

		~Camera() = default;

		Camera(const Camera&) = delete;
		Camera& operator=(const Camera&) = delete;
		Camera(Camera&&) = delete;
		Camera& operator=(Camera&&) = delete;

	private:
		glm::vec3 m_pos;
	};
}
