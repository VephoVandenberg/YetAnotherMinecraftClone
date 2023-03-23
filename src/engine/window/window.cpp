#include <iostream>

#include "../event/event.h"

#include "window.h"

constexpr float g_windowWidth = 1200;
constexpr float g_windowHeight = 800;

using namespace Engine;

Window::Window(CallbackData data)
	: m_width(g_windowWidth)
	, m_height(g_windowHeight)
	, m_data(data)
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

	m_window = glfwCreateWindow(m_width, m_height, "YetAnotherMinecraftClone", nullptr, nullptr);
	glfwMakeContextCurrent(m_window);
	glfwSetWindowUserPointer(m_window, &m_data);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR::GLAD_FAILED_TO_LOAD" << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(
		m_window,
		[](GLFWwindow* window, int key, int scancode, int action, int mods) {
			auto* data = static_cast<CallbackData*>(glfwGetWindowUserPointer(window));

			KeyboardEvent event;
			event.m_action = action;
			event.m_key = key;

			data->m_func(event);
		});

	glfwSetWindowCloseCallback(
		m_window,
		[](GLFWwindow* window) {
			auto* data = static_cast<CallbackData*>(glfwGetWindowUserPointer(window));
			
			CloseEvent event;

			data->m_func(event);
		}
	);
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