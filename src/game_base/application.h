#pragma once

#include <memory>
#include <vector>

#include "../engine/window/window.h"
#include "../engine/event/event.h"
#include "../engine/mesh/mesh.h"

#include "../game_modules/block/block.h"
#include "../game_modules/chunk/chunk.h"

#include "player.h"

namespace GameNamespace
{
	using namespace Engine;
	using namespace GameModules;

	class Application
	{
	public:
		Application();
		~Application() = default;

		void run();

		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(const Application&&) = delete;
		Application& operator=(Application&&) = delete;

	private:
		void init();
		void handleEvents(Event& event);
		void initShaders();
		void initTextures();
		void initTextureCubes();
		void initAtlas();
		void initChunks();
		void updateMeshes();

		float m_deltaFrame;
		float m_previousFrame;

		bool m_isRunning;

		std::vector<Chunk> m_chunks;

		std::unique_ptr<Window> m_window;

		std::unique_ptr<Player> m_player;	
	};
}
