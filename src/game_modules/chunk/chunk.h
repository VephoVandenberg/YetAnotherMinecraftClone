#pragma once

#include <vector>
#include <array>

#include <glm/glm.hpp>

#include "../../engine/mesh/mesh.h"

namespace Engine
{
	class Shader;
	class Texture;
	class TextureCube;
}

namespace GameModules
{
	using namespace Engine;
	class Block;

	class Chunk
	{
	public:
		Chunk(glm::vec3 pos);

		void setMesh();
		void draw(Shader& shader, TextureCube& texture, glm::mat4 cameraView);
		void updateToNeighbourChunk(Chunk& chunk);

		inline glm::vec3 getSize() const { return m_size; }
		
		Chunk() = default;
		~Chunk() = default;
		Chunk(Chunk&&) = default;
		Chunk& operator=(Chunk&&) = default;

		Chunk(const Chunk&) = delete;
		Chunk& operator=(const Chunk&) = delete;	

	private:
		void initBlocks();
		void setChunkFaces();
		void traverseChunkFaceX(Chunk& chunk, const unsigned int currentX, const unsigned int neighbourX);
		void traverseChunkFaceZ(Chunk& chunk, const unsigned int currentZ, const unsigned int neighbourZ);
		bool checkAir(unsigned int index);

		glm::vec3 m_size;
		glm::vec3 m_pos;

		Mesh m_mesh;

		struct BlockRenderData
		{
			Block block;

			bool front;
			bool back;
			bool top;
			bool bottom;
			bool right;
			bool left;

			BlockRenderData(Block&& b)
				: block(std::move(b))
				, front(false)
				, back(false)
				, top(false)
				, bottom(false)
				, right(false)
				, left(false)
			{}
		};
		std::vector<BlockRenderData> m_blocks;
	};
}