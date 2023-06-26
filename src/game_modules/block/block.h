#pragma once

#include <array>

#include <glm/glm.hpp>

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
		GrassDirt,
		Stone,
		Sand,
		Dirt,
		Snow,
		Wood,
		Leaf
	};

	using namespace Engine;
	using namespace Data;

	class Block
	{
	public:
		bool front;
		bool back;
		bool top;
		bool bottom;
		bool right;
		bool left;

		float topT_ind = 0.0f;
		float bottomT_ind = 0.0f;
		float sideT_ind = 0.0f;

	public:
		Block(glm::vec3 pos, BlockType type);

		BlockType getType() const { return m_type; }
		glm::vec3 getPos() const { return m_pos; }

		Block() = default;
		~Block() = default;
		Block(Block&& b) = default;
		Block& operator=(Block&&) = default;

		Block(const Block&) = delete;
		Block& operator=(const Block&) = delete;

	private:
		glm::vec3 m_pos;
		BlockType m_type;
	};
}
