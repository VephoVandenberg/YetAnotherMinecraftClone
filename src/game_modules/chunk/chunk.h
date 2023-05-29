#pragma once

#include <unordered_map>
#include <thread>
#include <array>

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
			return std::hash<int>()(v.x) ^ std::hash<int>()(v.z);
		}

		bool operator()(const glm::vec3& a, const glm::vec3& b) const
		{
			return a.x == b.x && a.x == b.z;
		}
	};

	class Chunk
	{
	public:
		Chunk(glm::vec3 pos, int octaves, float persistance);

		void initMesh();
		void initBlocks(int octaves, float persistance);
		void initMeshData();
		void setMesh();

		void draw(Shader& shader, TextureArray& texture, glm::mat4 cameraView);
		void setChunkFaces();
		void updateToNeighbourChunk(Chunk& chunk);
		bool processRayToRemoveBlock(Ray& ray);

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
		float myPerlin(float x, float z);
		void traverseChunkFaceX(Chunk& chunk, const unsigned int currentX, const unsigned int neighbourX);
		void traverseChunkFaceZ(Chunk& chunk, const unsigned int currentZ, const unsigned int neighbourZ);
		void calcBlockBorderData(
			const Block& block, const Ray& ray,
			float& tMaxX, float& tMaxY, float& tMaxZ,
			int& stepX, int& stepY, int& stepZ);
		void addVertices(Block& m_block);
		void checkSurroundedBlocks(int Z, int Y, int X);
		bool checkAir(int index);

		glm::vec3 m_size;
		glm::vec3 m_pos;

		Mesh m_mesh;

		bool m_meshInitialized = false;

		std::vector<Block> m_blocks;
		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_indicies;
	};
}