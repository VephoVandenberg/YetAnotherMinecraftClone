#pragma once

#include <glm/glm.hpp>

namespace Engine
{
	struct KeyboardEvent;

	class Camera
	{
	public:
		Camera(); 

		void update(float dt);
		void handleInput(KeyboardEvent event);

		inline glm::mat4 getCameraView() const { return m_cameraView; }

		~Camera() = default;

		Camera(const Camera&) = delete;
		Camera& operator=(const Camera&) = delete;
		Camera(Camera&&) = delete;
		Camera& operator=(Camera&&) = delete;

	private:
		glm::vec3 m_pos;
		glm::vec3 m_velocity;
		glm::mat4 m_cameraView;
	};
}
