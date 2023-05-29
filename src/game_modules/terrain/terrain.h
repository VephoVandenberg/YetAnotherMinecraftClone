#pragma once

#include <unordered_map>
#include <queue>
#include <future>
#include <glm/glm.hpp>

#include "../chunk/chunk.h"

namespace GameModules
{
	class Player;

	class Terrain
	{
	public:
		Terrain(glm::vec3 borderMin, glm::vec3 borderMax);

		void update();
		void draw(const glm::mat4& player);

		Terrain() = default;
		~Terrain() = default;
		Terrain(Terrain&&) = default;
		Terrain& operator=(Terrain&&) = default;

		Terrain(const Terrain&) = delete;
		Terrain& operator=(const Terrain&) = delete;

	private:
		void init();
		void setChunksMeshes();

		void updateTerrainOnX(
			float maxX, float minX,
			float maxZ, float minZ,
			float offsetX, float offsetZ);
		void updateTerrainOnZ(
			float maxX, float minX,
			float maxZ, float minZ,
			float offsetX, float offsetZ);

		bool isNotUpdatedX;
		bool isNotUpdatedZ;

		std::unordered_map<glm::vec3, Chunk, KeyFuncs> m_chunks;
		std::queue<Chunk*> m_chunksToInit;
		glm::vec3 m_borderMin;
		glm::vec3 m_borderMax;
	};
}

