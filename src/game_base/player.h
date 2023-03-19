#pragma once

#include "../engine/camera/camera.h"

namespace GameModule
{
	using namespace Engine;

	class Player
	{
	public:

		~Player() = default;

		Player() = delete;
		Player(const Player&) = delete;
		Player& operator=(const Player&) = delete;
		Player(Player&&) = delete;
		Player& operator=(Player&&) = delete;

	private:
		Camera m_camera;
	};
}
