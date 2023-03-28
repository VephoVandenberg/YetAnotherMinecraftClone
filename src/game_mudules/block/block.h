#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "../../engine/mesh/data.h"

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

		// void render(Renderer& renderer, Shader& shader, glm::mat4 view);

		inline std::vector<Data::Vertex> getVertices() const { return m_vertices; }

		~Block() = default;
		Block(Block&&) = default;
		Block& operator=(Block&&) = default;

		Block() = delete;
		Block(const Block&) = delete;
		Block& operator=(const Block&) = delete;

	private:
		glm::vec3 m_pos;
		Texture& m_texture;

		std::vector<Data::Vertex> m_vertices;
	};
}
