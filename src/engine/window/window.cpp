
#include <iostream>

#include "window.h"

constexpr size_t g_width = 1080;
constexpr size_t g_height = 720;

GLFWwindow* Engine::initWindow()
{
	if (!glfwInit())
	{
		std::cout << "Could not init GLFW" << std::endl;
		return nullptr;
	}


	GLFWwindow* window =
		glfwCreateWindow(g_width, g_height, "Another iteration of Minecraft", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return nullptr;
	}
	
	if (!window)
	{
		std::cout << "Failed to initialize GLFW window" << std::endl;
	}
	

	return window;
}