#pragma once

#include <unordered_map>
#include <thread>
#include <array>
#include <bit>

#include <glm/glm.hpp>

#include "../../engine/mesh/mesh.h"

#include "../block/block.h"

namespace Engine
{
	class Shader;
	class TextureArray;
	class Ray;
}

namespace GameModules
{
	using namespace Engine;

	struct KeyFuncs
	{
		size_t operator()(const glm::vec3& v) const
		{
			size_t h = 0xcf234123f;
			h ^= std::_Bit_cast<uint32_t>(v.x) + 0x9e3779b9 + (h << 6) + (h >> 2);
			h ^= std::_Bit_cast<uint32_t>(v.y) + 0x9e3779b9 + (h << 6) + (h >> 2);
			h ^= std::_Bit_cast<uint32_t>(v.z) + 0x9e3779b9 + (h << 6) + (h >> 2);
			return h;
		}

		bool operator()(const glm::vec3& a, const glm::vec3& b) const
		{
			return a.x == b.x && a.y == b.y && a.x == b.z;
		}
	};

	enum class RayStatus
	{
		EndInChunk,
		HitTheBlock,
		EndInNeighbour
	};

	class Chunk
	{
	public:
		Chunk(glm::vec3 pos);

		void initMesh();
		void initBlocks();
		void initMeshData();
		void setMesh();

		void draw(Shader& shader, TextureArray& texture, glm::mat4 cameraView);
		void setChunkFaces();
		void updateToNeighbourChunk(Chunk& chunk);
		bool processRayToRemoveBlock(const glm::vec3 traversePos);

		inline glm::vec3 getSize() const { return m_size; }
		inline bool isMeshInitialized() const { return m_meshInitialized;  }

		Chunk() = default;
		~Chunk() = default;
		Chunk(Chunk&&) = default;
		Chunk& operator=(Chunk&&) = default;

		Chunk(const Chunk&) = delete;
		Chunk& operator=(const Chunk&) = delete;	

	private:
		float perlin(float x, float z);
		void traverseChunkFaceX(Chunk& chunk, const unsigned int currentX, const unsigned int neighbourX);
		void traverseChunkFaceZ(Chunk& chunk, const unsigned int currentZ, const unsigned int neighbourZ);
		void addVertices(Block& m_block);
		void checkSurroundedBlocks(int Z, int Y, int X);
		bool checkAir(int index);
		BlockType getBlockType(int height, int index);

		glm::vec3 m_size;
		glm::vec3 m_pos;

		Mesh m_mesh;

		bool m_meshInitialized = false;

		std::vector<Block> m_blocks;
		std::vector<Vertex> m_vertices;
	};
}