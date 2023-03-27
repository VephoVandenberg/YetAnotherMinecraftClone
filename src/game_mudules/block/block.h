#pragma once

// #include "../../engine/texture/texture"

#include <glm/glm.hpp>

namespace Engine
{
	class Renderer;
	class Shader;
	class Texture;
}

namespace GameModules
{
	using namespace Engine;

	class Block
	{
	public:
		Block(glm::vec3 pos, Texture& texture);

		void render(Renderer& renderer, Shader& shader, glm::mat4 view);

		~Block() = default;
		Block(Block&&) = default;
		Block& operator=(Block&&) = default;

		Block() = delete;
		Block(const Block&) = delete;
		Block& operator=(const Block&) = delete;

	private:
		glm::vec3 m_pos;
		Texture& m_texture;
	};
}
