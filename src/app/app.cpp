#include "../engine/window/window.h"

#include "app.h"

using namespace Engine;
using namespace App;

Application::Application()
	: m_isRunning(true)
{
	init();
}

void Application::init()
{
	m_window = initWindow();
}

void Application::run()
{
	while (!glfwWindowShouldClose(m_window))
	{
		glClearColor(0, 1, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
}
