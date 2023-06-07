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
		Dirt
	};

	using namespace Engine;
	using namespace Data;

	class Block
	{
	public:
		bool front = false;
		bool back = false;
		bool top = false;
		bool bottom = false;
		bool right = false;
		bool left = false;

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
