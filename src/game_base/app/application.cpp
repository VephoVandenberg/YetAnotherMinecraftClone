
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <mutex>
#include <chrono>

#if _DEBUG
#include "../../engine/debug_subsystem/debug_windows.h"
#endif

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

#if _DEBUG
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#endif


using namespace Engine;
using namespace GameNamespace;


constexpr glm::vec3 g_chunkSize = glm::vec3(16.0f, 256.0f, 16.0f);
constexpr glm::vec3 g_borderStartMin = {-12 * g_chunkSize.x, 0.0f, -12 * g_chunkSize.z};
constexpr glm::vec3 g_borderStartMax = { 12 * g_chunkSize.x, 0.0f,  12 * g_chunkSize.z };

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
}

void Application::init()
{
	CallbackData data;
	data.m_func = std::bind(&Application::handleEvents, this, std::placeholders::_1);

	m_window = std::unique_ptr<Window>(new Window(data));
	m_player = std::unique_ptr<Player>(
		new Player(glm::vec3(0.0f, 140.0f, 0.0f), m_window->getWidth(), m_window->getHeight()));
	m_terrain = std::unique_ptr<Terrain>(new Terrain(g_borderStartMin, g_borderStartMax));
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
		m_player->handleInput(event, m_deltaFrame);
		break;
	}
}

void Application::initShaders()
{
	glm::mat4 projection = 
		glm::perspective(glm::radians(45.0f), m_window->getWidth() / m_window->getHeight(), 1.0f, 200.0f);
	glm::mat4 model = glm::mat4(1.0f);
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
			.getShader(sName).setMat4vf(Uniforms::g_model, model);
		ResourceManager::getInstance()
			.getShader(sName).setMat4vf(Uniforms::g_view, m_player->getCameraView());
	}
}

void Application::initTextures()
{
	for (auto& tValue : g_texturePaths)
	{
		auto& tName = tValue.first;		// Get texture name
		auto& tPath = tValue.second;	// Get texture source path

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

void Application::run()
{
	while (m_isRunning)
	{
		float currentFrame = glfwGetTime();
		m_deltaFrame = currentFrame - m_previousFrame;
		m_previousFrame = currentFrame;
#if _DEBUG
		DebugWindows::getInstance().setFramerate();
#endif
		m_window->clear();
		onDraw();
		onUpdate();

#if _DEBUG
		DebugWindows::getInstance().getFramerate();
#endif

		m_window->update();

	}
}

void Application::onUpdate()
{
	m_terrain->checkTerrainBorders(m_player->getPlayerPosition(), m_player->getVelocity());
	m_player->update(m_deltaFrame);
	m_terrain->update(*m_player);
}

void Application::onDraw()
{
	m_terrain->draw(m_player->getCameraView());
}
