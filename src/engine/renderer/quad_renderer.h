#pragma once

namespace Engine
{
	namespace GameModule
	{
		class Quad;
	}

	struct Buffer
	{
		unsigned int VAO;
		unsigned int VBO;
		unsigned int IBO;
	};

	class QuadRenderer
	{
	public:
		QuadRenderer();
		~QuadRenderer() = default;

		void render();

		QuadRenderer(const QuadRenderer&) = delete;
		QuadRenderer& operator=(const QuadRenderer&) = delete;
		QuadRenderer(QuadRenderer&&) = delete;
		QuadRenderer& operator=(QuadRenderer&&) = delete;

	private:
		void init();

		Buffer m_buffer;
	};
}
