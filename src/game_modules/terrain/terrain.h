#pragma once

#include <unordered_map>
#include <queue>
#include <future>
#include <glm/glm.hpp>

#include "../chunk/chunk.h"

namespace GameNamespace
{
	class Player;
}

namespace GameModules
{
	class Terrain
	{
	public:
		Terrain(glm::vec3 borderMin, glm::vec3 borderMax);
		
		void checkTerrainBorders(const glm::vec3 pos, const glm::vec3 velocity);

		void update(const GameNamespace::Player& player);
		void draw(const glm::mat4& view);

		Terrain() = default;
		~Terrain() = default;
		Terrain(Terrain&&) = default;
		Terrain& operator=(Terrain&&) = default;

		Terrain(const Terrain&) = delete;
		Terrain& operator=(const Terrain&) = delete;

	private:
		void init();

		void updateTerrainOnX(
			float maxX, float minX,
			float maxZ, float minZ,
			float offsetX, float offsetZ);
		void updateTerrainOnZ(
			float maxX, float minX,
			float maxZ, float minZ,
			float offsetX, float offsetZ);

		bool m_isNotUpdatedX = true;
		bool m_isNotUpdatedZ = true;

		std::unordered_map<glm::vec3, Chunk, KeyFuncs> m_chunks;
		std::queue<Chunk*> m_chunksToInit;
		std::queue<std::future<void>> m_futures;
		glm::vec3 m_borderMin;
		glm::vec3 m_borderMax;
	};
}

