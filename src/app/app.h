#pragma once

struct GLFWwindow;

namespace App
{
	class Application
	{
	public:
		Application();
		~Application() = default;

		void run();

		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(Application&) = delete;
		Application& operator=(Application&&) = delete;

	private:
		void init();

		bool m_isRunning = false;
		GLFWwindow* m_window;
	};
}
