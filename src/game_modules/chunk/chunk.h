#pragma once

#if USE_VECTOR
#include <vector>
#else
#include <unordered_map>
#endif


#include <glm/glm.hpp>

#include "../../engine/mesh/mesh.h"

namespace Engine
{
	class Shader;
	class TextureArray;
	class Ray;
}

namespace GameModules
{
	using namespace Engine;
	class Block;

	struct KeyFuncs
	{
		size_t operator()(const glm::vec3& v) const
		{
			return std::hash<int>()(v.x) ^ std::hash<int>()(v.y);
		}

		bool operator()(const glm::vec3& a, const glm::vec3& b) const
		{
			return a.x == b.x && a.y == b.y;
		}
	};

	class Chunk
	{
	public:
		Chunk(glm::vec3 pos);

		void initMesh();
		void setMesh();

		void draw(Shader& shader, TextureArray& texture, glm::mat4 cameraView);
		void setChunkFaces();
		void updateToNeighbourChunk(Chunk& chunk);
		bool processRayCast(Ray& ray);

		inline glm::vec3 getSize() const { return m_size; }
		
		Chunk() = default;
		~Chunk() = default;
		Chunk(Chunk&&) = default;
		Chunk& operator=(Chunk&&) = default;

		Chunk(const Chunk&) = delete;
		Chunk& operator=(const Chunk&) = delete;	

	private:
		void initBlocks();
		void initMeshData(std::vector<Vertex>& vertices, std::vector<unsigned int>& indicies);
		void traverseChunkFaceX(Chunk& chunk, const unsigned int currentX, const unsigned int neighbourX);
		void traverseChunkFaceZ(Chunk& chunk, const unsigned int currentZ, const unsigned int neighbourZ);
#if 1
		bool checkAir(unsigned int index);
#else
#endif
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

#if USE_VECTOR
		std::vector<BlockRenderData> m_blocks;
#else
		std::unordered_map<glm::vec3, BlockRenderData> m_blocks;
#endif
	};
}