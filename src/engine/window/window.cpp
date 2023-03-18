#include <iostream>

#include "window.h"

constexpr float g_windowWidth = 1200;
constexpr float g_windowHeight = 800;

using namespace Engine;

Window::Window()
{
	init();
}

void Window::init()
{
	if (!glfwInit())
	{
		std::cout << "ERROR::GLFW_FAILED_TO_LOAD" << std::endl;
		exit(EXIT_FAILURE);
	}

	m_window = glfwCreateWindow(g_windowWidth, g_windowHeight, "YetAnotherMinecraftClone", nullptr, nullptr);
	glfwMakeContextCurrent(m_window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR::GLAD_FAILED_TO_LOAD" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void Window::clear()
{
	glClearColor(0.0f, 0.0f, 0.8f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Window::update()
{
	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

Window::~Window()
{
	glfwDestroyWindow(m_window);
}