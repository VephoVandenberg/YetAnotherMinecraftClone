#include <map>

#include <iostream>

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

constexpr glm::vec3 g_chunkSize = glm::vec3(16.0f, 16.0f, 16.0f);

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
	for (int z = 0; z < g_chunkSize.z; z++)
	{
		for (int y = 0; y < g_chunkSize.y; y++)
		{
			for (int x = 0; x < g_chunkSize.x; x++)
			{
				glm::vec3 pos = { x, y, z };

				auto type = (y > g_chunkSize.y/2.0f) ? BlockType::Air : BlockType::Grass;

				if (type == BlockType::Air && x == 0)
				{

				}

				m_blocks.emplace_back(
					Block(pos, type));
			}
		}
	}
}

void Application::initMeshes()
{
	std::vector<Data::Vertex> vertices;
	std::vector<unsigned int> indicies;

	auto addFront = [](std::vector<unsigned int>& indicies, unsigned int i) {
		indicies.push_back(24 * i + 0);
		indicies.push_back(24 * i + 1);
		indicies.push_back(24 * i + 2);
		indicies.push_back(24 * i + 1);
		indicies.push_back(24 * i + 3);
		indicies.push_back(24 * i + 2);
	};

	auto addBack = [](std::vector<unsigned int>& indicies, unsigned int i) {
		indicies.push_back(24 * i + 4);
		indicies.push_back(24 * i + 6);
		indicies.push_back(24 * i + 5);
		indicies.push_back(24 * i + 5);
		indicies.push_back(24 * i + 6);
		indicies.push_back(24 * i + 7);
	};

	auto addTop = [](std::vector<unsigned int>& indicies, unsigned int i) {
		indicies.push_back(24 * i + 8);
		indicies.push_back(24 * i + 9);
		indicies.push_back(24 * i + 10);
		indicies.push_back(24 * i + 9);
		indicies.push_back(24 * i + 11);
		indicies.push_back(24 * i + 10);
	};

	auto addBottom = [](std::vector<unsigned int>& indicies, unsigned int i) {
		indicies.push_back(24 * i + 12);
		indicies.push_back(24 * i + 14);
		indicies.push_back(24 * i + 13);
		indicies.push_back(24 * i + 13);
		indicies.push_back(24 * i + 14);
		indicies.push_back(24 * i + 15);
	};

	auto addLeft = [](std::vector<unsigned int>& indicies, unsigned int i) {
		indicies.push_back(24 * i + 16);
		indicies.push_back(24 * i + 17);
		indicies.push_back(24 * i + 18);
		indicies.push_back(24 * i + 17);
		indicies.push_back(24 * i + 19);
		indicies.push_back(24 * i + 18);
	};

	auto addRight = [](std::vector<unsigned int>& indicies, unsigned int i) {
		indicies.push_back(24 * i + 20);
		indicies.push_back(24 * i + 22);
		indicies.push_back(24 * i + 21);
		indicies.push_back(24 * i + 21);
		indicies.push_back(24 * i + 22);
		indicies.push_back(24 * i + 23);
	};
	
	int i = 0;
	for (int z = 0; z < g_chunkSize.z; z++)
	{
		for (int y = 0; y < g_chunkSize.y; y++)
		{
			for (int x = 0; x < g_chunkSize.x; x++)
			{
				unsigned int currentBlock = z * g_chunkSize.x * g_chunkSize.y + y * g_chunkSize.y + x;

				if (m_blocks[currentBlock].getType() == BlockType::Air)
				{
					continue;
				}

				int frontBlock  = (z + 1) * g_chunkSize.x * g_chunkSize.y + y * g_chunkSize.y + x;
				int backBlock   = (z - 1) * g_chunkSize.x * g_chunkSize.y + y * g_chunkSize.y + x;
				int topBlock    = z * g_chunkSize.x * g_chunkSize.y + (y + 1) * g_chunkSize.y + x;
				int bottomBlock = z * g_chunkSize.x * g_chunkSize.y + (y - 1) * g_chunkSize.y + x;
				int rightBlock  = z * g_chunkSize.x * g_chunkSize.y + y * g_chunkSize.y + (x + 1);
				int leftBlock   = z * g_chunkSize.x * g_chunkSize.y + y * g_chunkSize.y + (x - 1);

				bool isFrontFree  = (z == g_chunkSize.z - 1);
				bool isBackFree   = (z == 0);
				bool isTopFree    = (y == g_chunkSize.y - 1);
				bool isBottomFree = (y == 0);
				bool isRightFree  = (x == g_chunkSize.x - 1);
				bool isLeftFree   = (x == 0);
				
				// Check for air blocks
				if (!isFrontFree)
				{
					isFrontFree = m_blocks[frontBlock].getType() == BlockType::Air;
				}
				if (!isBackFree)
				{
					isBackFree = m_blocks[backBlock].getType() == BlockType::Air;
				}
				if (!isTopFree)
				{
					isTopFree = m_blocks[topBlock].getType() == BlockType::Air;
				}
				if (!isBottomFree)
				{
					isBottomFree = m_blocks[bottomBlock].getType() == BlockType::Air;
				}
				if (!isLeftFree)
				{
					isLeftFree = m_blocks[leftBlock].getType() == BlockType::Air;
				}
				if (!isRightFree)
				{
					isRightFree = m_blocks[rightBlock].getType() == BlockType::Air;
				}
				
				// If overall results are free
				if (isFrontFree)
				{
					addFront(indicies, i);
				}
				if (isBackFree)
				{
					addBack(indicies, i);
				}
				if (isTopFree)
				{
					addTop(indicies, i);
				}
				if (isBottomFree)
				{
					addBottom(indicies, i);
				}
				if (isLeftFree)
				{
					addLeft(indicies, i);
				}
				if (isRightFree)
				{
					addRight(indicies, i);
				}
				
				i++;

				for (unsigned vIndex = 0;
					vIndex < m_blocks[currentBlock].getVertices().size();
					vIndex++)
				{
					vertices.emplace_back(m_blocks[currentBlock].getVertices()[vIndex]);
				}
			}
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

		m_meshes[0].draw(
			ResourceManager::getInstance().
			getShader(ShaderNames::g_base_shader),
			ResourceManager::getInstance().
			getTexture(TextureNames::g_grass),
			m_player->getCameraView());

		m_window->update();
	}
}
