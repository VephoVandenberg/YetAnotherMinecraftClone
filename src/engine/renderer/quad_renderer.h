#pragma once

#include <glm/glm.hpp>

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

	class Shader;
	class Texture;
	class QuadRenderer
	{
	public:
		QuadRenderer();
		~QuadRenderer() = default;

		void render(Shader& shader, Texture& texture, glm::mat4 view);

		QuadRenderer(const QuadRenderer&) = delete;
		QuadRenderer& operator=(const QuadRenderer&) = delete;
		QuadRenderer(QuadRenderer&&) = delete;
		QuadRenderer& operator=(QuadRenderer&&) = delete;

	private:
		void init();

		Buffer m_buffer;
	};
}
