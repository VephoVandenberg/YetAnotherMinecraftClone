
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <mutex>
#include <chrono>

#include "../../engine/resource_manager/resource_manager.h"
#include "../../engine/shader/shader.h"
#include "../../engine/texture/texture.h"
#include "../../engine/ray/ray.h"
#include "../../engine/texture/texture_array.h"
#include "../../engine/shader/shader_list.h"
#include "../../engine/shader/uniform_list.h"
#include "../../engine/texture/texture_list.h"
#include "../../engine/texture/texture_cube_list.h"
#include "../../engine/event/event.h"

#include "application.h"

using namespace Engine;
using namespace GameNamespace;

constexpr glm::vec3 g_chunkSize = glm::vec3(16.0f, 256.0f, 16.0f);
constexpr float g_rayMagnitude = 7.0f;

std::mutex g_chunkMap_lock;

Application::Application()
	: m_isRunning(true)
{
	init();
	initShaders();
	initTextures();
	initTextureArray();
	initTextureCubes();
	initChunks();
}

void Application::init()
{
	CallbackData data;
	data.m_func = std::bind(&Application::handleEvents, this, std::placeholders::_1);

	m_window = std::unique_ptr<Window>(new Window(data));
	m_player = std::unique_ptr<Player>(
		new Player(glm::vec3(0.0f, 40.0f, 0.0f),
			m_window->getWidth(), m_window->getHeight()));
}

void Application::handleEvents(Event& event)
{
	switch (event.getType())
	{
	case EventType::Close:
	{
		m_isRunning = false;
	}break;

	default:
		m_player->handleInput(event);
		break;
	}
}

void Application::initShaders()
{
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), m_window->getWidth() / m_window->getHeight(), 1.0f, 200.0f);
	for (auto& sValue : g_shaderPaths)
	{
		auto& sName = sValue.first;				// Get shader name
		auto& vPath = sValue.second.first;		// Get vertex source path
		auto& fPath = sValue.second.second;		// Get fragment source path

		ResourceManager::getInstance()
			.setShader(sName, vPath, fPath);

		ResourceManager::getInstance()
			.getShader(sName).setMat4vf(Uniforms::g_projection, projection);
		ResourceManager::getInstance()
			.getShader(sName).setMat4vf(Uniforms::g_view, m_player->getCameraView());
	}
}

void Application::initTextures()
{
	for (auto& tValue : g_texturePaths)
	{
		auto& tName = tValue.first;
		auto& tPath = tValue.second;

		ResourceManager::getInstance()
			.setTexture(tName, tPath);
	}
}

void Application::initTextureCubes()
{
	for (auto& tValue : g_textureCubePaths)
	{
		auto& tName = tValue.first;
		auto& fPaths = tValue.second;

		ResourceManager::getInstance()
			.setTextureCube(tName, fPaths);
	}
}

void Application::initTextureArray()
{
	ResourceManager::getInstance()
		.setTextureArray(s_texturePaths);
}

void Application::initChunks()
{
	borderMin = { -6.0f * g_chunkSize.x, 0.0f, -6.0 * g_chunkSize.z };
	borderMax = { 6.0f * g_chunkSize.x, 0.0f,  6.0 * g_chunkSize.z };

	for (float z = borderMin.z; z < borderMax.z; z += g_chunkSize.z)
	{
		for (float x = borderMin.x; x < borderMax.x; x += g_chunkSize.x)
		{
			glm::vec3 pos = { x, 0.0f, z };

			m_chunks[pos] = std::move(Chunk(pos));

			//auto begin = std::chrono::high_resolution_clock::now();
			m_chunks[pos].initBlocks();
			//auto end = std::chrono::high_resolution_clock::now();

#if 0
			std::cout << "initBlocks - "
				<< std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
#endif

			//begin = std::chrono::high_resolution_clock::now();
			m_chunks[pos].setChunkFaces();
			//end = std::chrono::high_resolution_clock::now();

#if 0
			std::cout << "SetChunkFaces - " 
				<< std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
#endif
		}
	}
	checkChunksNeighbours();
	setChunksMeshes();
}

void Application::checkChunksNeighbours()
{
	for (auto& chunk : m_chunks)
	{
		glm::vec3 posNX = chunk.first; posNX.x += g_chunkSize.x;
		if (m_chunks.find(posNX) != m_chunks.end())
		{
			//auto begin = std::chrono::high_resolution_clock::now();
			chunk.second.updateToNeighbourChunk(m_chunks[posNX]);
			//auto end = std::chrono::high_resolution_clock::now();
#if 0
			std::cout << "checkXNeighbour - " << 
				std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
#endif
		}

		glm::vec3 posNZ = chunk.first; posNZ.z += g_chunkSize.z;
		if (m_chunks.find(posNZ) != m_chunks.end())
		{
			//auto begin = std::chrono::high_resolution_clock::now();
			chunk.second.updateToNeighbourChunk(m_chunks[posNZ]);
			//auto end = std::chrono::high_resolution_clock::now();
#if 0
			std::cout << "checkZNeighbour - " << 
				std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
#endif
		}
	}
}

void Application::checkTerrainBorders()
{
	auto dir = m_player->getVelocity();

	int signX = (dir.x > 0) ? 1 : -1;
	int signZ = (dir.z > 0) ? 1 : -1;
	float distanceX = 
		(signX > 0) ?
		((borderMax.x - borderMin.x) / 2.0f - g_chunkSize.x / 2.0f):
		((borderMin.x - borderMax.x) / 2.0f - g_chunkSize.x / 2.0f);
	float distanceZ =
		(signZ > 0) ?
		((borderMax.z - borderMin.z) / 2.0f - g_chunkSize.z / 2.0f) :
		((borderMin.z - borderMax.z) / 2.0f - g_chunkSize.z / 2.0f);
	float maxX = signX > 0 ? borderMax.x : borderMin.x - g_chunkSize.x;
	float minX = signX > 0 ? borderMin.x : borderMax.x - g_chunkSize.x;
	float maxZ = signZ > 0 ? borderMax.z : borderMin.z - g_chunkSize.z;
	float minZ = signZ > 0 ? borderMin.z : borderMax.z - g_chunkSize.z;
	float offsetX = signX * g_chunkSize.x;
	float offsetZ = signZ * g_chunkSize.z;

	bool exprX = false;
	bool exprZ = false;


	if (dir.x > 0.0f)
	{
		exprX = maxX - m_player->getPlayerPosition().x < distanceX;
	}
	else
	{
		exprX = maxX - m_player->getPlayerPosition().x > distanceX;
	}

	if (exprX)
	{
		m_futures.push(
			std::async(
				std::launch::async, &Application::updateTerrainOnX, 
				this, 
				maxX, minX, maxZ, minZ, offsetX, offsetZ, signX, signZ));
		borderMax.x += offsetX;
		borderMin.x += offsetX;
	}

	if (dir.z > 0.0f) 
	{
		exprZ = maxZ - m_player->getPlayerPosition().z < distanceZ;
	}
	else
	{
		exprZ = maxZ - m_player->getPlayerPosition().z > distanceZ;
	}

	if (exprZ)
	{
		m_futures.push(
			std::async(
				std::launch::async, &Application::updateTerrainOnZ, 
				this, 
				maxX, minX, maxZ, minZ, offsetX, offsetZ, signX, signZ));
		borderMax.z += offsetZ;
		borderMin.z += offsetZ;
	}
}

void Application::updateTerrainOnX(
	float maxX, float minX,
	float maxZ, float minZ,
	float offsetX, float offsetZ,
	int signX, int signZ)
{
	isNotUpdatedX = false;
	bool shouldUpdateZ = true;

	float z = minZ;

	while (z != maxZ)
	{
		glm::vec3 pos = { maxX, 0.0f, z };
		Chunk chunk(pos);
		chunk.initBlocks();
		chunk.setChunkFaces();

		if (shouldUpdateZ && !isNotUpdatedZ)
		{
			//maxZ += offsetZ;
			//borderMax.z += offsetZ;
			//shouldUpdateZ = false;
		}

		glm::vec3 posNX = { maxX - offsetX, 0.0f, z };
		if (m_chunks.find(posNX) != m_chunks.end())
		{
			g_chunkMap_lock.lock();
			chunk.updateToNeighbourChunk(m_chunks[posNX]);
			g_chunkMap_lock.unlock();
		}

		glm::vec3 posNZ = { maxX, 0.0f, z - g_chunkSize.z };
		if (m_chunks.find(posNZ) != m_chunks.end())
		{
			g_chunkMap_lock.lock();
			chunk.updateToNeighbourChunk(m_chunks[posNZ]);
			g_chunkMap_lock.unlock();
		}

		g_chunkMap_lock.lock();
		m_chunks.erase(glm::vec3(minX, 0.0f, z));
		chunk.initMeshData();
		m_chunks[pos] = std::move(chunk);
		m_chunksToInit.push(&m_chunks[pos]);
		g_chunkMap_lock.unlock();

		z += offsetZ;
	}

	g_chunkMap_lock.lock();
	m_chunks[glm::vec3(maxX, 0.0f, z)] = std::move(Chunk(glm::vec3(maxX, 0.0f, z)));
	m_chunks.erase(glm::vec3(minX, 0.0f, z));
	g_chunkMap_lock.unlock();

	isNotUpdatedX = true;
}

void Application::updateTerrainOnZ(
	float maxX, float minX,
	float maxZ, float minZ,
	float offsetX, float offsetZ,
	int signX, int signZ)
{
	isNotUpdatedZ = false;
	bool shouldUpdateX = true;

	float x = minX;

	while (x != maxX)
	{
		glm::vec3 pos = { x, 0.0f, maxZ };
		Chunk chunk(pos);
		chunk.initBlocks();
		chunk.setChunkFaces();

		if (shouldUpdateX && !isNotUpdatedX)
		{
			//maxX += offsetX;
			//borderMax.x += offsetX;
			//shouldUpdateX = false;
		}

		glm::vec3 posNX = { x - g_chunkSize.x, 0.0f, maxZ };
		if (m_chunks.find(posNX) != m_chunks.end())
		{
			g_chunkMap_lock.lock();
			chunk.updateToNeighbourChunk(m_chunks[posNX]);
			g_chunkMap_lock.unlock();
		}

		glm::vec3 posNZ = { x, 0.0f, maxZ - offsetZ };
		if (m_chunks.find(posNZ) != m_chunks.end())
		{
			g_chunkMap_lock.lock();
			chunk.updateToNeighbourChunk(m_chunks[posNZ]);
			g_chunkMap_lock.unlock();
		}
		g_chunkMap_lock.lock();
		m_chunks.erase(glm::vec3(x, 0.0f, minZ));
		chunk.initMeshData();
		m_chunks[pos] = std::move(chunk);
		m_chunksToInit.push(&m_chunks[pos]);
		g_chunkMap_lock.unlock();

		x += offsetX;
	}

	g_chunkMap_lock.lock();
	m_chunks[glm::vec3(x, 0.0f, maxZ)] = std::move(Chunk(glm::vec3(x, 0.0f, maxZ)));
	m_chunks.erase(glm::vec3(x, 0.0f, minZ));
	g_chunkMap_lock.unlock();

	isNotUpdatedZ = true;
}

void Application::setChunksMeshes()
{
	for (auto& chunk : m_chunks)
	{
		chunk.second.initMeshData();
		chunk.second.initMesh();
#if 0
		std::cout << "initMesh - " <<
			std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
#endif
	}
}

void Application::run()
{
	while (m_isRunning)
	{
		float currentFrame = glfwGetTime();
		m_deltaFrame = currentFrame - m_previousFrame;
		m_previousFrame = currentFrame;

		m_window->clear();

		m_player->update(m_deltaFrame);
		checkTerrainBorders();
		updateChunks();
		drawChunks();

		m_window->update();
	}
}

void Application::drawChunks()
{
	for (auto& chunk : m_chunks)
	{
		chunk.second.draw(
			ResourceManager::getInstance().getShader(ShaderNames::g_base_shader),
			ResourceManager::getInstance().getTextureArray(),
			m_player->getCameraView());
	}
}

void Application::updateChunks()
{
	if (m_player->getLeftButtonStatus())
	{
		glm::vec3 rayPos = {
			m_player->getPlayerPosition().x + 0.5f,
			m_player->getPlayerPosition().y,
			m_player->getPlayerPosition().z + 0.5f
		};

		Ray ray(
			rayPos,
			m_player->getCameraFront(),
			g_rayMagnitude);

		glm::vec3 pos = {
			static_cast<int>(m_player->getPlayerPosition().x)
				- static_cast<int>(m_player->getPlayerPosition().x) % static_cast<int>(g_chunkSize.x),
			0.0f,
			static_cast<int>(m_player->getPlayerPosition().z)
				- static_cast<int>(m_player->getPlayerPosition().z) % static_cast<int>(g_chunkSize.z)
		};

		bool rayEndsInChunk =
			ray.getEndPoint().x >= pos.x && ray.getEndPoint().x <= pos.x + g_chunkSize.x &&
			ray.getEndPoint().z >= pos.z && ray.getEndPoint().z <= pos.z + g_chunkSize.z;

		if (m_chunks[pos].processRayToRemoveBlock(ray))
		{
			if (!rayEndsInChunk)
			{
				// Another chunk should be procecessed
			}

			glm::vec3 positiveX = pos;
			glm::vec3 positiveZ = pos;
			glm::vec3 negativeX = pos;
			glm::vec3 negativeZ = pos;

			positiveX.x += g_chunkSize.x;
			negativeX.x -= g_chunkSize.x;
			if (m_chunks.find(positiveX) != m_chunks.end())
			{
				m_chunks[pos].updateToNeighbourChunk(m_chunks[positiveX]);
			}
			if (m_chunks.find(negativeX) != m_chunks.end())
			{
				m_chunks[pos].updateToNeighbourChunk(m_chunks[negativeX]);
			}

			positiveZ.z += g_chunkSize.z;
			negativeZ.z -= g_chunkSize.z;
			if (m_chunks.find(positiveZ) != m_chunks.end())
			{
				m_chunks[pos].updateToNeighbourChunk(m_chunks[positiveZ]);
			}
			if (m_chunks.find(negativeZ) != m_chunks.end())
			{
				m_chunks[pos].updateToNeighbourChunk(m_chunks[negativeZ]);
			}

			m_chunks[positiveX].initMeshData();
			m_chunks[negativeX].initMeshData();
			m_chunks[positiveZ].initMeshData();
			m_chunks[negativeZ].initMeshData();
			m_chunks[positiveX].setMesh();
			m_chunks[negativeX].setMesh();
			m_chunks[positiveZ].setMesh();
			m_chunks[negativeZ].setMesh();
			m_chunks[pos].setMesh();
		}
	}

	if (!m_chunksToInit.empty())
	{
		m_chunksToInit.front()->initMesh();
		m_chunksToInit.pop();
	}
}
