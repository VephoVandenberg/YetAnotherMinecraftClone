#pragma once

#include "../../engine/camera/camera.h"

namespace Engine
{
	// class Chunk;
	class Event;
}

namespace GameNamespace
{
	class Player
	{
	public:
		Player(float width, float height);

		void update(float dt);
		void handleInput(Engine::Event& event);
		
		inline glm::mat4 getCameraView() const { return m_camera.getCameraView(); }
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
