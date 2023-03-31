#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "../../engine/texture/texture.h"
#include "../../engine/mesh/data.h"

namespace Engine
{
	class Renderer;
	class Shader;
}

namespace GameModules
{
	enum class BlockType
	{
		Air = 0,
		Grass,
		Stone,
		Dirt
	};

	using namespace Engine;

	class Block
	{
	public:
		Block(glm::vec3 pos, BlockType type);

		inline const std::vector<Data::Vertex>& getVertices() const { return m_vertices; }
		inline const BlockType getType() const { return m_type; }

		~Block() = default;
		Block(Block&&) = default;
		Block& operator=(Block&&) = default;

		Block() = delete;
		Block(const Block&) = delete;
		Block& operator=(const Block&) = delete;

	private:
		glm::vec3 m_pos;
		BlockType m_type;
		Texture m_texture;

		std::vector<Data::Vertex> m_vertices;
	};
}
