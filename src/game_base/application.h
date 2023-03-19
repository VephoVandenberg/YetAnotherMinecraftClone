#pragma once

#include <memory>

#include "../engine/window/window.h"
#include "../engine/renderer/quad_renderer.h"

namespace GameNamespace
{
	using namespace Engine;

	class Application
	{
	public:
		Application();
		~Application() = default;

		void run();

		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(const Application&&) = delete;
		Application& operator=(Application&&) = delete;

	private:
		void init();
		void initShaders();
		void initTextures();

		bool m_isRunning;

		std::unique_ptr<Window> m_window;
		std::unique_ptr<QuadRenderer> m_renderer;
	};
}
