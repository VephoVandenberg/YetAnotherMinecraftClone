#include <map>

#include <iostream>

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
constexpr float g_rayMagnitude = 5.0f;

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
	for (unsigned int i = 0; i < 5; i++)
	{
		m_chunks.emplace_back(Chunk(glm::vec3(i*16.0f, 1.0f, 0.0f)));
	}
	
	checkChunksNeighbours();
	updateChunkMeshes();
}

void Application::checkChunksNeighbours()
{
	for (unsigned int i = 0; i < m_chunks.size() - 1; i++)
	{
		m_chunks[i].updateToNeighbourChunk(m_chunks[i + 1]);
	}
}

void Application::updateChunkMeshes()
{
	for (unsigned int i = 0; i < m_chunks.size(); i++)
	{
		m_chunks[i].setMesh();
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

		m_window->update();
	}
}

void Application::updateChunks()
{
	for (auto& chunk : m_chunks)
	{
		if (m_player->getLeftButtonStatus())
		{
			Ray ray(
				m_player->getPlayerPosition(),
				m_player->getCameraFront(),
				g_rayMagnitude);
			
			if (chunk.processRayCast(ray));
			{
				chunk.setChunkFaces();
				checkChunksNeighbours();
				updateChunkMeshes();
			}
		}

		chunk.draw(
			ResourceManager::getInstance().getShader(ShaderNames::g_base_shader),
			ResourceManager::getInstance().getTextureArray(),
			m_player->getCameraView());
	}
}

