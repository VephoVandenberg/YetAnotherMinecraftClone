#pragma once

#include <glm/glm.hpp>

namespace Engine
{
	struct KeyboardEvent;
	struct MouseMoveEvent;

#define BIT(x) 1 << x

	enum class MoveType
	{
		None  = 0,
		Front = BIT(0),
		Back  = BIT(1),
		Left  = BIT(2),
		Right = BIT(4),
	};

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
		glm::mat4 m_cameraView;

		bool m_firstMove;

		float m_lastX;
		float m_lastY;

		float m_yaw = -90.0f;
		float m_pitch;

		unsigned int m_moveType;
	};
}
