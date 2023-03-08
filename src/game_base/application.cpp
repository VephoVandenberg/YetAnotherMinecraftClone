#include "application.h"

using namespace GAME_NAMESPACE;

Application::Application()
	: m_isRunning(true)
{
	init();
}

void Application::init()
{
	m_window = std::unique_ptr<Window>(new Window);
	m_renderer = std::unique_ptr<QuadRenderer>(new QuadRenderer);
}

void Application::run()
{
	while (m_isRunning)
	{
		m_window->clear();

		m_renderer->render();
		
		m_window->update();
	}
}
