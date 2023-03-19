#include <map>

#include "../engine/resource_manager/resource_manager.h"
#include "../engine/shader/shader_list.h"
#include "../engine/texture/texture_list.h"

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
	m_window = std::unique_ptr<Window>(new Window);
	m_renderer = std::unique_ptr<QuadRenderer>(new QuadRenderer);
}

void Application::initShaders()
{
	for (auto& sValue : g_shaderPaths)
	{
		auto& sName = sValue.first;
		auto& vPath = sValue.second.first;
		auto& fPath = sValue.second.second;

		ResourceManager::getInstance()
			.setShader(sName, vPath, fPath);
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
	while (m_isRunning)
	{
		m_window->clear();
		
		m_renderer->render(
			ResourceManager::getInstance()
				.getShader(ShaderNames::g_base_shader),
			ResourceManager::getInstance()
				.getTexture(TextureNames::g_grass));
		
		m_window->update();
	}
}
