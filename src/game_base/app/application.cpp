#include <glm/gtc/matrix_transform.hpp>

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
	m_player = std::unique_ptr<Player>(new Player(m_window->getWidth(), m_window->getHeight()));
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

	for (unsigned int z = 0; z < 5; z++)
	{
		for (unsigned int x = 0; x < 5; x++)
		{
			glm::vec3 pos = { x * g_chunkSize.x, 0.0f, z * g_chunkSize.z };
			m_chunks[pos] = std::move(Chunk(pos));
		}
	}
	checkChunksNeighbours();
	updateChunkMeshes();
}

void Application::checkChunksNeighbours()
{
	for (unsigned int z = 0; z < 5; z++)
	{
		for (unsigned int x = 0; x < 5; x++)
		{
			glm::vec3 pos = { x * g_chunkSize.x, 0.0f, z * g_chunkSize.z };
			glm::vec3 posNX = { x * g_chunkSize.x + g_chunkSize.x, 0.0f, z * g_chunkSize.z };
			glm::vec3 posNZ = { x * g_chunkSize.x, 0.0f, z * g_chunkSize.z + g_chunkSize.z };

			if (m_chunks.find(posNX) != m_chunks.end())
			{
				m_chunks[pos].updateToNeighbourChunk(m_chunks[posNX]);
			}
			if (m_chunks.find(posNZ) != m_chunks.end())
			{
				m_chunks[pos].updateToNeighbourChunk(m_chunks[posNZ]);
			}
		}
	}
}

void Application::updateChunkMeshes()
{
	for (unsigned int z = 0; z < 5; z++)
	{
		for (unsigned int x = 0; x < 5; x++)
		{
			glm::vec3 pos = { x * g_chunkSize.x, 0.0f, z * g_chunkSize.z };
			m_chunks[pos].initMesh();
		}
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
	for (unsigned int z = 0; z < 5; z++)
	{
		for (unsigned int x = 0; x < 5; x++)
		{
			if (m_player->getLeftButtonStatus())
			{
				glm::vec3 pos = { x * g_chunkSize.x, 0.0f, z * g_chunkSize.z };

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
							// Another chunk should be worked out
						}

						glm::vec3 positiveX = { (x + 1) * g_chunkSize.x, 0.0f, z * g_chunkSize.z };
						glm::vec3 positiveZ = { x * g_chunkSize.x, 0.0f, (z + 1) * g_chunkSize.z };
						glm::vec3 negativeX = { (x - 1) * g_chunkSize.x, 0.0f, z * g_chunkSize.z };
						glm::vec3 negativeZ = { x * g_chunkSize.x, 0.0f, (z - 1) * g_chunkSize.z };


						if (m_chunks.find(positiveX) != m_chunks.end())
						{
							m_chunks[pos].updateToNeighbourChunk(m_chunks[positiveX]);
						}
						if (m_chunks.find(negativeX) != m_chunks.end())
						{
							m_chunks[pos].updateToNeighbourChunk(m_chunks[negativeX]);
						}

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
						return;
					}
				}
			}
		}
	}
}
