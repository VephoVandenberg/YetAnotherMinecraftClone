#pragma once

#include <vector>

#include <glm/glm.hpp>


namespace GameModules
{
	class Block;

	class Tree
	{
	public:
		Tree(glm::vec3 pos);

		Tree() = default;
		~Tree() = default;
		Tree(Tree&&) = default;
		Tree& operator=(Tree&&) = default;

		Tree(const Tree&) = delete;
		Tree& operator=(const Tree&) = delete;

	private:
		glm::vec3 m_pos;
	};
}
