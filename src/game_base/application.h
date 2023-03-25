#pragma once

#include <memory>
#include <deque>

#include "../engine/window/window.h"
#include "../engine/renderer/quad_renderer.h"
#include "../engine/event/event.h"

#include "player.h"

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
		void handleEvents(Event& event);
		void initShaders();
		void initTextures();

		float m_deltaFrame;
		float m_previousFrame;

		bool m_isRunning;

		std::deque<Event*> m_eventQueue;

		std::unique_ptr<Window> m_window;
		std::unique_ptr<QuadRenderer> m_renderer;

		std::unique_ptr<Player> m_player;	
	};
}
