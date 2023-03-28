#include <map>

#include <glm/gtc/matrix_transform.hpp>

#include "../engine/resource_manager/resource_manager.h"
#include "../engine/shader/shader.h"
#include "../engine/shader/shader_list.h"
#include "../engine/shader/uniform_list.h"
#include "../engine/texture/texture_list.h"
#include "../engine/event/event.h"

#include "application.h"

using namespace Engine;
using namespace GameNamespace;

Application::Application()
	: m_isRunning(true)
{
	init();
	initShaders();
	initTextures();
	initBlocks();
	initMeshes();
}

void Application::init()
{
	CallbackData data;
	data.m_func = std::bind(&Application::handleEvents, this, std::placeholders::_1);

	m_window = std::unique_ptr<Window>(new Window(data));
	// m_renderer = std::unique_ptr<Renderer>(new Renderer);
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

void Application::initBlocks()
{
	unsigned int xRange = 10;
	unsigned int yRange = 10;
	unsigned int zRange = 15;

	for (unsigned int x = 0; x < xRange; x++)
	{
		for (unsigned int y = 0; y < yRange; y++)
		{
			for (unsigned int z = 0; z < zRange; z++)
			{
				glm::vec3 pos = {
					x, y, z
				};
				m_blocks.emplace_back(
					Block(pos, ResourceManager::getInstance().getTexture(TextureNames::g_grass)));
			}
		}
	}
}

void Application::initMeshes()
{
	unsigned int numberOfMesh = 1;

	std::vector<Data::Vertex> vertices;
	std::vector<unsigned int> indicies;

	for (unsigned int i = 0; i < m_blocks.size(); i++)
	{
		// front
		indicies.push_back(24 * i + 0);
		indicies.push_back(24 * i + 1);
		indicies.push_back(24 * i + 2);
		indicies.push_back(24 * i + 1);
		indicies.push_back(24 * i + 3);
		indicies.push_back(24 * i + 2);

		// back
		indicies.push_back(24 * i + 4);
		indicies.push_back(24 * i + 6);
		indicies.push_back(24 * i + 5);
		indicies.push_back(24 * i + 5);
		indicies.push_back(24 * i + 6);
		indicies.push_back(24 * i + 7);

		// top
		indicies.push_back(24 * i + 8);
		indicies.push_back(24 * i + 9);
		indicies.push_back(24 * i + 10);
		indicies.push_back(24 * i + 9);
		indicies.push_back(24 * i + 11);
		indicies.push_back(24 * i + 10);

		// bottom
		indicies.push_back(24 * i + 12);
		indicies.push_back(24 * i + 14);
		indicies.push_back(24 * i + 13);
		indicies.push_back(24 * i + 13);
		indicies.push_back(24 * i + 14);
		indicies.push_back(24 * i + 15);

		// left
		indicies.push_back(24 * i + 16);
		indicies.push_back(24 * i + 17);
		indicies.push_back(24 * i + 18);
		indicies.push_back(24 * i + 17);
		indicies.push_back(24 * i + 19);
		indicies.push_back(24 * i + 18);

		// right
		indicies.push_back(24 * i + 20);
		indicies.push_back(24 * i + 22);
		indicies.push_back(24 * i + 21);
		indicies.push_back(24 * i + 21);
		indicies.push_back(24 * i + 22);
		indicies.push_back(24 * i + 23);

		for (unsigned int vIndex = 0; vIndex < m_blocks[i].getVertices().size(); vIndex++)
		{
			vertices.push_back(m_blocks[i].getVertices()[vIndex]);
		}
	}
	m_meshes.emplace_back(Mesh(vertices, indicies));
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

		/*
		for (auto& block : m_blocks)
		{
			block.render(
				*m_renderer,
				ResourceManager::getInstance().
					getShader(ShaderNames::g_base_shader),
				m_player->getCameraView());
		}
		*/

		/*
		m_renderer->render(glm::vec3(0.0f),
			ResourceManager::getInstance().
			getShader(ShaderNames::g_base_shader),
			ResourceManager::getInstance().
			getTexture(TextureNames::g_grass),
			m_player->getCameraView());
			*/
		m_meshes[0].draw(
			ResourceManager::getInstance().
			getShader(ShaderNames::g_base_shader),
			ResourceManager::getInstance().
			getTexture(TextureNames::g_grass),
			m_player->getCameraView());

		m_window->update();
	}
}
