#pragma once

#include "../engine/camera/camera.h"

namespace GameNamespace
{
	using namespace Engine;

	class Engine::Event;

	class Player
	{
	public:
		Player();

		void update(float dt);
		void handleInput(Event& event);
		
		inline glm::mat4 getCameraView() const { return m_camera.getCameraView(); }

		~Player() = default;

		Player(const Player&) = delete;
		Player& operator=(const Player&) = delete;
		Player(Player&&) = delete;
		Player& operator=(Player&&) = delete;

	private:
		Camera m_camera;
	};
}
