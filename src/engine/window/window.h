#pragma once

#include <glad/glad.h>
#include <glfw3.h>

namespace Engine
{
	class Window
	{
	public:
		Window();
		~Window();

		void clear();
		void update();

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator=(Window&&) = delete;

	private:
		void init();

		GLFWwindow* m_window;
	};
}

