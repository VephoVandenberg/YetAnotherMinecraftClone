#pragma once

#include "../../engine/camera/camera.h"

namespace Engine
{
	class Event;
}

namespace GameNamespace
{
	class Player
	{
	public:
		Player(glm::vec3 pos, float width, float height);

		void update(float dt);
		void handleInput(Engine::Event& event, float dt);
		
		inline glm::mat4 getCameraView() const { return m_camera.getCameraView(); }
		inline glm::vec3 getCameraFront() const { return m_camera.getCameraFront(); }
		inline glm::vec3 getPlayerPosition() const { return m_camera.getCameraPosition(); }
		inline glm::vec3 getVelocity() const { return m_camera.getVelocity(); }
		inline bool getLeftButtonStatus() const { return m_leftButtonClicked; }

		~Player() = default;

		Player(const Player&) = delete;
		Player& operator=(const Player&) = delete;
		Player(Player&&) = delete;
		Player& operator=(Player&&) = delete;

	private:
		Engine::Camera m_camera;

		bool m_leftButtonClicked = false;
	};
}
