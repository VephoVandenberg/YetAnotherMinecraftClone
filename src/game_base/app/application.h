#pragma once

#include <memory>
#include <unordered_map>
#include <queue>
#include <future>

#include "../../engine/window/window.h"
#include "../../engine/event/event.h"
//#include "../../engine/mesh/mesh.h"

#include "../../game_modules/block/block.h"
#include "../../game_modules/chunk/chunk.h"

#include "../player/player.h"

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
		void initShaders();
		void initTextures();
		void initTextureCubes();
		void initTextureArray();
		void initChunks();

		void checkTerrainBorders();

		void updateTerrainOnX(
			float maxX, float minX,
			float maxZ, float minZ, 
			float offsetX, float offsetZ);
		void updateTerrainOnZ(
			float maxX, float minX, 
			float maxZ, float minZ, 
			float offsetX, float offsetZ);

		void drawChunks();

		void updateChunks();
		void setChunksMeshes();

		void handleEvents(Event& event);

		float m_deltaFrame;
		float m_previousFrame;

		bool m_isRunning;

		glm::vec3 borderMin;
		glm::vec3 borderMax;

		std::unordered_map<glm::vec3, Chunk, KeyFuncs> m_chunks;
		std::queue<std::future<void>> m_futures;
		std::queue<Chunk*> m_chunksToInit;

		std::unique_ptr<Window> m_window;

		std::unique_ptr<Player> m_player;

		bool isNotUpdatedX = true;
		bool isNotUpdatedZ = true;
	};
}
