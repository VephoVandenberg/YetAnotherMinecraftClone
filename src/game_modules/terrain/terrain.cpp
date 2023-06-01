#include <mutex>

#include "../../game_base/player/player.h"

#include "../../engine/resource_manager/resource_manager.h"
#include "../../engine/shader/shader_list.h"
#include "../../engine/ray/ray.h"

#include "terrain.h"

using namespace GameModules;

constexpr glm::vec3 g_chunkSize = glm::vec3(16.0f, 256.0f, 16.0f);
constexpr float g_rayMagnitude = 4.0f;

std::mutex g_chunk_lock;

Terrain::Terrain(glm::vec3 borderMin, glm::vec3 borderMax)
	: m_borderMin(borderMin)
	, m_borderMax(borderMax)
{
	init();
}

void Terrain::init()
{
	for (float z = m_borderMin.z; z < m_borderMax.z; z += g_chunkSize.z)
	{
		for (float x = m_borderMin.x; x < m_borderMax.x; x += g_chunkSize.x)
		{
			glm::vec3 pos = { x, 0.0f, z };

			m_chunks[pos] = std::move(Chunk(pos));
			m_chunks[pos].setChunkFaces();

			glm::vec3 posNX = { x - g_chunkSize.x, 0.0f, z };
			if (m_chunks.find(posNX) != m_chunks.end())
			{
				m_chunks[pos].updateToNeighbourChunk(m_chunks[posNX]);
			}

			glm::vec3 posNZ = { x, 0.0f, z - g_chunkSize.z };
			if (m_chunks.find(posNZ) != m_chunks.end())
			{
				m_chunks[pos].updateToNeighbourChunk(m_chunks[posNZ]);
			}
		}
	}

	for (auto& chunk : m_chunks)
	{
		chunk.second.initMeshData();
		chunk.second.initMesh();
	}
}

void Terrain::draw(const glm::mat4& view)
{
	for (auto& chunk : m_chunks)
	{
		bool canDraw = chunk.second.isMeshInitialized();

		{
			chunk.second.draw(
				ResourceManager::getInstance().getShader(ShaderNames::g_base_shader),
				ResourceManager::getInstance().getTextureArray(),
				view);
		}
	}
}

void Terrain::checkTerrainBorders(const glm::vec3 pos, const glm::vec3 velocity)
{
	int signX = (velocity.x > 0) ? 1 : -1;
	int signZ = (velocity.z > 0) ? 1 : -1;
	float distanceX =
		std::abs((m_borderMax.x - m_borderMin.x)) / 2.0f - g_chunkSize.x / 2.0f;
	float distanceZ =
		std::abs((m_borderMax.z - m_borderMin.z)) / 2.0f - g_chunkSize.z / 2.0f;
	float maxX = signX > 0 ? m_borderMax.x : m_borderMin.x - g_chunkSize.x;
	float minX = signX > 0 ? m_borderMin.x : m_borderMax.x - g_chunkSize.x;
	float maxZ = signZ > 0 ? m_borderMax.z : m_borderMin.z - g_chunkSize.z;
	float minZ = signZ > 0 ? m_borderMin.z : m_borderMax.z - g_chunkSize.z;
	float offsetX = signX * g_chunkSize.x;
	float offsetZ = signZ * g_chunkSize.z;

	bool exprX = false;
	bool exprZ = false;

	exprX = std::abs(maxX - pos.x) < distanceX;
	if (exprX)
	{
		m_futures.push(
			std::async(
				std::launch::async, &Terrain::updateTerrainOnX,
				this,
				maxX, minX, maxZ, minZ, offsetX, offsetZ));
		m_borderMax.x += offsetX;
		m_borderMin.x += offsetX;
	}

	exprZ = std::abs(maxZ - pos.z) < distanceZ;
	if (exprZ)
	{
		m_futures.push(
			std::async(
				std::launch::async, &Terrain::updateTerrainOnZ,
				this,
				maxX, minX, maxZ, minZ, offsetX, offsetZ));
		m_borderMax.z += offsetZ;
		m_borderMin.z += offsetZ;
	}
}

void Terrain::update(const GameNamespace::Player& player)
{
	if (!m_chunksToInit.empty())
	{
		if (m_chunksToInit.front()->isMeshInitialized())
		{
			m_chunksToInit.front()->setMesh();
		}
		else
		{
			m_chunksToInit.front()->initMesh();
		}
		if (m_chunksToInit.front())
		{
			m_chunksToInit.pop();
		}
	}

	if (player.getLeftButtonStatus())
	{
		glm::vec3 rayPos = {
			player.getPlayerPosition().x + 0.5f,
			player.getPlayerPosition().y,
			player.getPlayerPosition().z + 0.5f
		};

		Ray ray(
			rayPos,
			player.getCameraFront(),
			g_rayMagnitude);

		glm::vec3 pos = {
			static_cast<int>((player.getPlayerPosition().x))
				- static_cast<int>((player.getPlayerPosition().x)) % static_cast<int>(g_chunkSize.x),
			0.0f,
			static_cast<int>(std::floor(player.getPlayerPosition().z))
				- static_cast<int>((player.getPlayerPosition().z)) % static_cast<int>(g_chunkSize.z)
		};

		glm::vec3 positiveX = pos;
		glm::vec3 positiveZ = pos;
		glm::vec3 negativeX = pos;
		glm::vec3 negativeZ = pos;

		positiveX.x += g_chunkSize.x;
		negativeX.x -= g_chunkSize.x;
		positiveZ.z += g_chunkSize.z;
		negativeZ.z -= g_chunkSize.z;

		switch (m_chunks[pos].processRayToRemoveBlock(ray))
		{
		case RayStatus::HitTheBlock:
		{
			m_chunks[pos].updateToNeighbourChunk(m_chunks[positiveX]);
			m_chunks[pos].updateToNeighbourChunk(m_chunks[negativeX]);

			m_chunks[pos].updateToNeighbourChunk(m_chunks[positiveZ]);
			m_chunks[pos].updateToNeighbourChunk(m_chunks[negativeZ]);

			m_chunks[positiveX].initMeshData();
			m_chunks[negativeX].initMeshData();
			m_chunks[positiveZ].initMeshData();
			m_chunks[negativeZ].initMeshData();
			m_chunks[pos].initMeshData();
			m_chunks[positiveX].setMesh();
			m_chunks[negativeX].setMesh();
			m_chunks[positiveZ].setMesh();
			m_chunks[negativeZ].setMesh();
			m_chunks[pos].setMesh();
		} break;

		case RayStatus::EndInNeighbour:
		{
			bool isInChunk =
				ray.getEndPoint().x >= positiveX.x && ray.getEndPoint().x < positiveX.x + g_chunkSize.x &&
				ray.getEndPoint().z >= positiveX.z && ray.getEndPoint().z < positiveX.z + g_chunkSize.z;
			if (isInChunk)
			{
				if (m_chunks[positiveX].processRayToRemoveBlock(ray) == RayStatus::HitTheBlock)
				{
					glm::vec3 posNX = { positiveX.x, 0.0f, positiveX.z + g_chunkSize.z };
					glm::vec3 negNX = { positiveX.x, 0.0f, positiveX.z - g_chunkSize.z };
					m_chunks[pos].updateToNeighbourChunk(m_chunks[positiveX]);
					m_chunks[positiveX].updateToNeighbourChunk(m_chunks[posNX]);
					m_chunks[positiveX].updateToNeighbourChunk(m_chunks[negNX]);
					m_chunks[positiveX].initMeshData();
					m_chunks[posNX].initMeshData();
					m_chunks[negNX].initMeshData();
					m_chunks[pos].initMeshData();
					m_chunks[positiveX].setMesh();
					m_chunks[posNX].setMesh();
					m_chunks[negNX].setMesh();
					m_chunks[pos].setMesh();
				}
			}
			isInChunk =
				ray.getEndPoint().x >= negativeX.x && ray.getEndPoint().x < negativeX.x + g_chunkSize.x &&
				ray.getEndPoint().z >= negativeX.z && ray.getEndPoint().z < negativeX.z + g_chunkSize.z;
			if (isInChunk)
			{
				if (m_chunks[negativeX].processRayToRemoveBlock(ray) == RayStatus::HitTheBlock)
				{
					glm::vec3 posNX = { negativeX.x, 0.0f, negativeX.z + g_chunkSize.z };
					glm::vec3 negNX = { negativeX.x, 0.0f, negativeX.z - g_chunkSize.z };
					m_chunks[pos].updateToNeighbourChunk(m_chunks[negativeX]);
					m_chunks[negativeX].updateToNeighbourChunk(m_chunks[posNX]);
					m_chunks[negativeX].updateToNeighbourChunk(m_chunks[negNX]);
					m_chunks[negativeX].initMeshData();
					m_chunks[posNX].initMeshData();
					m_chunks[negNX].initMeshData();
					m_chunks[pos].initMeshData();
					m_chunks[negativeX].setMesh();
					m_chunks[posNX].setMesh();
					m_chunks[negNX].setMesh();
					m_chunks[pos].setMesh();
				}
			}
			isInChunk =
				ray.getEndPoint().x >= positiveZ.x && ray.getEndPoint().x < positiveZ.x + g_chunkSize.x &&
				ray.getEndPoint().z >= positiveZ.z && ray.getEndPoint().z < positiveZ.z + g_chunkSize.z;
			if (isInChunk)
			{
				if (m_chunks[positiveZ].processRayToRemoveBlock(ray) == RayStatus::HitTheBlock)
				{
					glm::vec3 posNZ = { positiveZ.x + g_chunkSize.x, 0.0f, positiveZ.z};
					glm::vec3 negNZ = { positiveZ.x - g_chunkSize.x, 0.0f, positiveZ.z};
					m_chunks[pos].updateToNeighbourChunk(m_chunks[positiveX]);
					m_chunks[positiveX].updateToNeighbourChunk(m_chunks[posNZ]);
					m_chunks[positiveX].updateToNeighbourChunk(m_chunks[negNZ]);
					m_chunks[positiveX].initMeshData();
					m_chunks[posNZ].initMeshData();
					m_chunks[negNZ].initMeshData();
					m_chunks[pos].initMeshData();
					m_chunks[positiveX].setMesh();
					m_chunks[posNZ].setMesh();
					m_chunks[negNZ].setMesh();
					m_chunks[pos].setMesh();
				}
			}
			isInChunk =
				ray.getEndPoint().x >= negativeZ.x && ray.getEndPoint().x < negativeZ.x + g_chunkSize.x &&
				ray.getEndPoint().z >= negativeZ.z && ray.getEndPoint().z < negativeZ.z + g_chunkSize.z;
			if (isInChunk)
			{
				if (m_chunks[negativeZ].processRayToRemoveBlock(ray) == RayStatus::HitTheBlock)
				{
					glm::vec3 posNZ = { negativeZ.x + g_chunkSize.x, 0.0f, negativeZ.z };
					glm::vec3 negNZ = { negativeZ.x - g_chunkSize.x, 0.0f, negativeZ.z };
					m_chunks[pos].updateToNeighbourChunk(m_chunks[negativeZ]);
					m_chunks[negativeZ].updateToNeighbourChunk(m_chunks[posNZ]);
					m_chunks[negativeZ].updateToNeighbourChunk(m_chunks[negNZ]);
					m_chunks[negativeZ].initMeshData();
					m_chunks[posNZ].initMeshData();
					m_chunks[negNZ].initMeshData();
					m_chunks[pos].initMeshData();
					m_chunks[negativeZ].setMesh();
					m_chunks[posNZ].setMesh();
					m_chunks[negNZ].setMesh();
					m_chunks[pos].setMesh();
				}
			}
		}
		}
	}
}

void Terrain::updateTerrainOnX(
	float maxX, float minX,
	float maxZ, float minZ,
	float offsetX, float offsetZ)
{
	m_isNotUpdatedX = false;

	float z = minZ;

	while (z != maxZ)
	{
		glm::vec3 pos = { maxX, 0.0f, z };
		g_chunk_lock.lock();
		m_chunks[pos] = std::move(Chunk(pos));
		m_chunks[pos].setChunkFaces();
		g_chunk_lock.unlock();

		glm::vec3 posNX = { maxX - offsetX, 0.0f, z };
		if (m_chunks.find(posNX) != m_chunks.end())
		{
			g_chunk_lock.lock();
			m_chunks[pos].updateToNeighbourChunk(m_chunks[posNX]);
			m_chunks[posNX].initMeshData();
			m_chunksToInit.push(&m_chunks[posNX]);
			g_chunk_lock.unlock();
		}

		glm::vec3 posNZ = { maxX, 0.0f, z - offsetZ };
		if (m_chunks.find(posNZ) != m_chunks.end())
		{
			g_chunk_lock.lock();
			m_chunks[pos].updateToNeighbourChunk(m_chunks[posNZ]);
			g_chunk_lock.unlock();
		}

		g_chunk_lock.lock();
		m_chunks.erase(glm::vec3(minX, 0.0f, z));
		g_chunk_lock.unlock();

		z += offsetZ;
	}

	m_isNotUpdatedX = true;
}

void Terrain::updateTerrainOnZ(
	float maxX, float minX,
	float maxZ, float minZ,
	float offsetX, float offsetZ)
{
	m_isNotUpdatedZ = false;

	float x = minX;

	while (x != maxX)
	{
		glm::vec3 pos = { x, 0.0f, maxZ };
		g_chunk_lock.lock();
		m_chunks[pos] = std::move(Chunk(pos));
		m_chunks[pos].setChunkFaces();
		g_chunk_lock.unlock();

		glm::vec3 posNX = { x - offsetX, 0.0f, maxZ };
		if (m_chunks.find(posNX) != m_chunks.end())
		{
			g_chunk_lock.lock();
			m_chunks[pos].updateToNeighbourChunk(m_chunks[posNX]);
			g_chunk_lock.unlock();
		}

		glm::vec3 posNZ = { x, 0.0f, maxZ - offsetZ };
		if (m_chunks.find(posNZ) != m_chunks.end())
		{
			g_chunk_lock.lock();
			m_chunks[pos].updateToNeighbourChunk(m_chunks[posNZ]);
			m_chunks[posNZ].initMeshData();
			m_chunksToInit.push(&m_chunks[posNZ]);
			g_chunk_lock.unlock();
		}

		g_chunk_lock.lock();
		m_chunks.erase(glm::vec3(x, 0.0f, minZ));
		g_chunk_lock.unlock();

		x += offsetX;
	}

	m_isNotUpdatedZ = true;
}
