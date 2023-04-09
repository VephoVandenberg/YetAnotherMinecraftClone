#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "../../engine/texture/texture_cube.h"
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
	using namespace Data;

	class Block
	{
	public:
		Block(glm::vec3 pos, BlockType type);

		inline const std::vector<Vertex>& getVertices() const { return m_vertices; }
		inline BlockType getType() const { return m_type; }
		inline glm::vec3 getPos() const { return m_pos; }

		~Block() = default;
		Block(Block&&) = default;
		Block& operator=(Block&&) = default;

		Block() = delete;
		Block(const Block&) = delete;
		Block& operator=(const Block&) = delete;

	private:
		glm::vec3 m_pos;
		BlockType m_type;
		TextureCube m_texture;

		std::vector<Vertex> m_vertices;
	};
}
