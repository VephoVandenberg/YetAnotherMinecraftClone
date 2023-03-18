#include <map>

#include "../engine/resource_manager/resource_manager.h"
#include "../engine/shader/shader_list.h"

#include "application.h"

using namespace Engine;
using namespace GameNamespace;

Application::Application()
	: m_isRunning(true)
{
	init();
	initShaders();
}

void Application::init()
{
	m_window = std::unique_ptr<Window>(new Window);
	m_renderer = std::unique_ptr<QuadRenderer>(new QuadRenderer);
}

void Application::initShaders()
{
	for (auto& value : g_shaderPaths)
	{
		ResourceManager::getInstance()
			.setShader(value.first, value.second.first, value.second.second);
	}
}

void Application::run()
{
	while (m_isRunning)
	{
		ResourceManager::getInstance().getShader(g_base_shader);
		m_window->clear();

		m_renderer->render();
		
		m_window->update();
	}
}
