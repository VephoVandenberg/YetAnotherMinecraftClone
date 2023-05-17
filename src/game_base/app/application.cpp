
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

std::vector<int> Application::generateHeightMap(glm::vec3 pos)
{
	std::vector<int> v;
	return v;
}

void Application::initChunks()
{
	borderMin = { -3.0f * g_chunkSize.x, 0.0f, -3.0 * g_chunkSize.z };
	borderMax = {  3.0f * g_chunkSize.x, 0.0f,  3.0 * g_chunkSize.z };

	for (float z = borderMin.z; z < borderMax.z; z += g_chunkSize.z)
	{
		for (float x = borderMin.x; x < borderMax.x; x += g_chunkSize.x)
		{
			glm::vec3 pos = { x, 0.0f, z };

			m_chunks[pos] = std::move(Chunk(pos));
			m_chunks[pos].initHeightMap();
			m_chunks[pos].initBlocks();
			m_chunks[pos].setChunkFaces();
			
		}
	}
	checkChunksNeighbours();
	updateChunkMeshes();
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
	// We've got out posstion in (x, y, z)
	// but mostly we are interested in x and z,
	// so, we then check for the velocity x and z components,
	// and then add some chunks to the terrain and remove the opposite chunks on the other side

	auto dir = m_player->getVelocity();

	glm::vec3 toMax = borderMax - m_player->getPlayerPosition();
	glm::vec3 toMin = borderMin - m_player->getPlayerPosition();

	if (toMax.x < 20.0f)
	{
		for (float z = borderMin.z; z < borderMax.z; z += g_chunkSize.z)
		{
			glm::vec3 pos = { borderMax.x, 0.0f, z };

			m_chunks[pos] = std::move(Chunk(pos));
			m_chunks[pos].initHeightMap();
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
				m_chunks[pos ].updateToNeighbourChunk(m_chunks[posNZ]);
			}
			m_chunks[pos].initMesh();
		}
		borderMax.x += g_chunkSize.x;
	}
	else if (dir.x < 0 && std::abs(toMin.x) < 5.0f)
	{
		
	}

	if (dir.z > 0 && std::abs(toMax.z) < 5.0f)
	{
		
	}
	else if (dir.z < 0 && std::abs(toMin.x) < 5.0f)
	{
		
	}

}

void Application::updateChunkMeshes()
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
	for (auto& chunk : m_chunks)
	{
		if (m_player->getLeftButtonStatus())
		{
			glm::vec3 pos = chunk.first;

			if (m_chunks.find(pos) == m_chunks.end())
			{
				continue;
			}

			glm::vec3 rayPos = {
				m_player->getPlayerPosition().x + 0.5f,
				m_player->getPlayerPosition().y,
				m_player->getPlayerPosition().z + 0.5f
			};

			Ray ray(
				rayPos,
				m_player->getCameraFront(),
				g_rayMagnitude);

			bool rayStartInChunk =
				ray.getPosition().x >= pos.x && ray.getPosition().x <= pos.x + g_chunkSize.x &&
				ray.getPosition().z >= pos.z && ray.getPosition().z <= pos.z + g_chunkSize.z;
			bool rayEndsInChunk =
				ray.getEndPoint().x >= pos.x && ray.getEndPoint().x <= pos.x + g_chunkSize.x &&
				ray.getEndPoint().z >= pos.z && ray.getEndPoint().z <= pos.z + g_chunkSize.z;

			if (rayStartInChunk)
			{
				if (m_chunks[pos].processRayToRemoveBlock(ray))
				{
					if (!rayEndsInChunk)
					{
						// Another chunk should be procecessed
					}

					glm::vec3 positiveX = chunk.first;
					glm::vec3 positiveZ = chunk.first;
					glm::vec3 negativeX = chunk.first;
					glm::vec3 negativeZ = chunk.first;

					positiveX.x += g_chunkSize.x;
					if (m_chunks.find(positiveX) != m_chunks.end())
					{
						m_chunks[pos].updateToNeighbourChunk(m_chunks[positiveX]);
					}

					negativeX.x -= g_chunkSize.x;
					if (m_chunks.find(negativeX) != m_chunks.end())
					{
						m_chunks[pos].updateToNeighbourChunk(m_chunks[negativeX]);
					}

					positiveZ.z += g_chunkSize.z;
					if (m_chunks.find(positiveZ) != m_chunks.end())
					{
						m_chunks[pos].updateToNeighbourChunk(m_chunks[positiveZ]);
					}

					negativeZ.z -= g_chunkSize.z;
					if (m_chunks.find(negativeZ) != m_chunks.end())
					{
						m_chunks[pos].updateToNeighbourChunk(m_chunks[negativeZ]);
					}

					m_chunks[positiveX].setMesh();
					m_chunks[negativeX].setMesh();
					m_chunks[positiveZ].setMesh();
					m_chunks[negativeZ].setMesh();
					m_chunks[pos].setMesh();
					return;
				}
			}
		}
	}
}
