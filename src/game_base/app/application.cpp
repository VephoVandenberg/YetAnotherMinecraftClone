
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

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
	m_player = std::unique_ptr<Player>(new Player(glm::vec3(0.0f, 40.0f, 0.0f), m_window->getWidth(), m_window->getHeight()));
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
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), m_window->getWidth() / m_window->getHeight(), 1.0f, 100.0f);
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
	borderMin = { -3.0f * g_chunkSize.x, 0.0f, -3.0 * g_chunkSize.z };
	borderMax = { 3.0f * g_chunkSize.x, 0.0f,  3.0 * g_chunkSize.z };

	for (float z = borderMin.z; z < borderMax.z; z += g_chunkSize.z)
	{
		for (float x = borderMin.x; x < borderMax.x; x += g_chunkSize.x)
		{
			glm::vec3 pos = { x, 0.0f, z };

			m_chunks[pos] = std::move(Chunk(pos));
			m_chunks[pos].initBlocks();
			m_chunks[pos].setChunkFaces();

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
			chunk.second.updateToNeighbourChunk(m_chunks[posNX]);
		}

		glm::vec3 posNZ = chunk.first; posNZ.z += g_chunkSize.z;
		if (m_chunks.find(posNZ) != m_chunks.end())
		{
			chunk.second.updateToNeighbourChunk(m_chunks[posNZ]);
		}
	}
}

void Application::checkTerrainBorders()
{
	auto dir = m_player->getVelocity();

	glm::vec3 toMax = borderMax - m_player->getPlayerPosition();
	glm::vec3 toMin = borderMin - m_player->getPlayerPosition();

	if (toMax.x < 20.0f)
	{
		for (float z = borderMin.z; z < borderMax.z; z += g_chunkSize.z)
		{
			glm::vec3 pos = { borderMax.x, 0.0f, z };

			m_chunks[pos] = std::move(Chunk(pos));
			m_chunks[pos].initBlocks();
			m_chunks[pos].setChunkFaces();

			glm::vec3 posNX = { borderMax.x - g_chunkSize.x, 0.0f, z };
			if (m_chunks.find(posNX) != m_chunks.end())
			{
				m_chunks[pos].updateToNeighbourChunk(m_chunks[posNX]);
			}

			glm::vec3 posNZ = { borderMax.x, 0.0f, z - g_chunkSize.z };
			if (m_chunks.find(posNZ) != m_chunks.end())
			{
				m_chunks[pos].updateToNeighbourChunk(m_chunks[posNZ]);
			}

			m_chunks.erase(glm::vec3(borderMin.x, 0.0f, z));
		}

		for (glm::vec3 pos = { borderMax.x, 0.0f, borderMin.z };
			pos.z < borderMax.z;
			pos.z += g_chunkSize.z)
		{
			m_chunks[pos].initMesh();
		}

		borderMax.x += g_chunkSize.x;
		borderMin.x += g_chunkSize.x;
	}
}

void Application::setChunksMeshes()
{
	for (auto& chunk : m_chunks)
	{
		chunk.second.initMesh();
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

			m_chunks[positiveX].setMesh();
			m_chunks[negativeX].setMesh();
			m_chunks[positiveZ].setMesh();
			m_chunks[negativeZ].setMesh();
			m_chunks[pos].setMesh();
		}
	}
}
