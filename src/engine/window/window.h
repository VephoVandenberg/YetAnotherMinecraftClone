#pragma once

#include <functional>

#if _DEBUG
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Engine
{
	struct Keyboard
	{
		bool keys[1024];
	};

	struct Event;

	struct CallbackData
	{
		using EventFn = std::function<void(Event&)>;
		EventFn m_func;
	};

	class Window
	{
	public:
		Window(CallbackData data);
		~Window();

		void clear();
		void update();

		inline float getWidth() const { return m_width; }
		inline float getHeight() const { return m_height; }
		
		Window() = delete;
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator=(Window&&) = delete;

	private:
		void init();

		float m_width;
		float m_height;

		GLFWwindow* m_window;

		CallbackData m_data;
	};
}

