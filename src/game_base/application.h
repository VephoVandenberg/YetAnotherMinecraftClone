#pragma once

#include <memory>
#include <vector>

#include "../engine/window/window.h"
#include "../engine/event/event.h"
#include "../engine/mesh/mesh.h"

#include "../game_mudules/block/block.h"

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
		void initBlocks();
		void initMeshes();

		float m_deltaFrame;
		float m_previousFrame;

		bool m_isRunning;

		std::vector<Block> m_blocks;
		std::vector<BlockType> m_blockTypes;
		std::vector<Mesh> m_meshes;

		std::unique_ptr<Window> m_window;

		std::unique_ptr<Player> m_player;	
	};
}
