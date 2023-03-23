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
}

void Application::init()
{
	CallbackData data;
	data.m_func = std::bind(&Application::handleEvents, this, std::placeholders::_1);

	m_window = std::unique_ptr<Window>(new Window(data));
	m_renderer = std::unique_ptr<QuadRenderer>(new QuadRenderer);
	m_player = std::unique_ptr<Player>(new Player());
}

void Application::handleEvents(Event& event)
{
	if (event.m_type == EventType::Close)
	{
		m_isRunning = false;
	}
	else
	{
		m_player->handleInput(event);
	}
}

void Application::initShaders()
{
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), m_window->getWidth() / m_window->getHeight(), 1.0f, 100.0f);
	for (auto& sValue : g_shaderPaths)
	{
		auto& sName = sValue.first;
		auto& vPath = sValue.second.first;
		auto& fPath = sValue.second.second;

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

void Application::run()
{
	float previousFrame = 0.0f;
	float currentFrame = 0.0f;
	while (m_isRunning)
	{
		float dt = currentFrame - previousFrame;
		previousFrame = currentFrame;
		currentFrame = glfwGetTime();

		m_window->clear();
		
		m_player->update(dt);

		m_renderer->render(
			ResourceManager::getInstance()
				.getShader(ShaderNames::g_base_shader),
			ResourceManager::getInstance()
				.getTexture(TextureNames::g_grass),
			m_player->getCameraView());
		
		m_window->update();
	}
}
