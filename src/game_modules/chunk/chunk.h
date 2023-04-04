#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "../../engine/mesh/mesh.h"

namespace Engine
{
	class Shader;
	class Texture;
}

namespace GameModules
{
	using namespace Engine;

	class Block;

	class Chunk
	{
	public:
		Chunk(glm::vec3 pos);

		void update();
		void draw(Shader& shader, Texture& texture, glm::mat4 cameraView);
		
		Chunk() = default;
		~Chunk() = default;
		Chunk(Chunk&&) = default;
		Chunk& operator=(Chunk&&) = default;

		Chunk(const Chunk&) = delete;
		Chunk& operator=(const Chunk&) = delete;	

	private:
		void initBlocks();
		void setChunkFaces();

		glm::vec3 m_size;
		glm::vec3 m_pos;

		Mesh m_mesh;
		std::vector<Block> m_blocks;

		std::vector<unsigned int> m_indicies;
		std::vector<Vertex> m_vertices;
	};
}