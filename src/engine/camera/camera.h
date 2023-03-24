#pragma once

#include <glm/glm.hpp>

namespace Engine
{
	struct KeyboardEvent;
	struct MouseMoveEvent;

	class Camera
	{
	public:
		Camera(float width, float height); 

		void update(float dt);
		void handleInput(KeyboardEvent event);
		void handleInput(MouseMoveEvent event);

		inline glm::mat4 getCameraView() const { return m_cameraView; }

		~Camera() = default;

		Camera(const Camera&) = delete;
		Camera& operator=(const Camera&) = delete;
		Camera(Camera&&) = delete;
		Camera& operator=(Camera&&) = delete;

	private:
		glm::vec3 m_pos;
		glm::vec3 m_cameraFront;
		glm::vec3 m_velocity;
		glm::mat4 m_cameraView;

		bool m_firstMove;

		float m_lastX;
		float m_lastY;

		float m_yaw = -90.0f;
		float m_pitch;
	};
}
