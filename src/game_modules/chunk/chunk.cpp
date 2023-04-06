#include <unordered_map>

#include "../../engine/shader/shader.h"
#include "../../engine/texture/texture.h"

#include "../block/block.h"

#include "chunk.h"

using namespace GameModules;

constexpr glm::vec3 g_chunkSize = { 16.0f, 256.0f, 16.0f };

auto addFront = [](std::vector<unsigned int>& indicies, unsigned int i) {
	indicies.push_back(24 * i + 0);
	indicies.push_back(24 * i + 1);
	indicies.push_back(24 * i + 2);
	indicies.push_back(24 * i + 1);
	indicies.push_back(24 * i + 3);
	indicies.push_back(24 * i + 2);
};

auto addBack = [](std::vector<unsigned int>& indicies, unsigned int i) {
	indicies.push_back(24 * i + 4);
	indicies.push_back(24 * i + 6);
	indicies.push_back(24 * i + 5);
	indicies.push_back(24 * i + 5);
	indicies.push_back(24 * i + 6);
	indicies.push_back(24 * i + 7);
};

auto addTop = [](std::vector<unsigned int>& indicies, unsigned int i) {
	indicies.push_back(24 * i + 8);
	indicies.push_back(24 * i + 9);
	indicies.push_back(24 * i + 10);
	indicies.push_back(24 * i + 9);
	indicies.push_back(24 * i + 11);
	indicies.push_back(24 * i + 10);
};

auto addBottom = [](std::vector<unsigned int>& indicies, unsigned int i) {
	indicies.push_back(24 * i + 12);
	indicies.push_back(24 * i + 14);
	indicies.push_back(24 * i + 13);
	indicies.push_back(24 * i + 13);
	indicies.push_back(24 * i + 14);
	indicies.push_back(24 * i + 15);
};

auto addLeft = [](std::vector<unsigned int>& indicies, unsigned int i) {
	indicies.push_back(24 * i + 16);
	indicies.push_back(24 * i + 17);
	indicies.push_back(24 * i + 18);
	indicies.push_back(24 * i + 17);
	indicies.push_back(24 * i + 19);
	indicies.push_back(24 * i + 18);
};

auto addRight = [](std::vector<unsigned int>& indicies, unsigned int i) {
	indicies.push_back(24 * i + 20);
	indicies.push_back(24 * i + 22);
	indicies.push_back(24 * i + 21);
	indicies.push_back(24 * i + 21);
	indicies.push_back(24 * i + 22);
	indicies.push_back(24 * i + 23);
};

Chunk::Chunk(glm::vec3 pos)
	: m_size(g_chunkSize)
	, m_pos(pos)
{
	initBlocks();
	setChunkFaces();
}

void Chunk::initBlocks()
{
	// Block initialization must be changed
	for (int z = 0; z < g_chunkSize.z; z++)
	{
		for (int y = 0; y < g_chunkSize.y; y++)
		{
			for (int x = 0; x < g_chunkSize.x; x++)
			{
				glm::vec3 pos = m_pos + glm::vec3(x, y, z);
				BlockType type = (y < 10 ? BlockType::Grass : BlockType::Air);

				m_blocks.emplace_back(BlockRenderData(Block(pos, type)));
			}
		}
	}
}

#define FRONT_FACE(x, y, z, size)	(z + 1) * g_chunkSize.y * g_chunkSize.x + y * g_chunkSize.x + x
#define BACK_FACE(x, y, z, size)	(z - 1) * g_chunkSize.y * g_chunkSize.x + y * g_chunkSize.x + x
#define TOP_FACE(x, y, z, size)		z * g_chunkSize.y * g_chunkSize.x + (y + 1) * g_chunkSize.x + x
#define BOTTOM_FACE(x, y, z, size)	z * g_chunkSize.y * g_chunkSize.x + (y - 1) * g_chunkSize.x + x
#define RIGHT_FACE(x, y, z, size)	z * g_chunkSize.y * g_chunkSize.x + y * g_chunkSize.x + (x + 1)
#define LEFT_FACE(x, y, z, sie)		z * g_chunkSize.y * g_chunkSize.x + y * g_chunkSize.x + (x - 1)

bool Chunk::checkAir(unsigned int index)
{
	if (index >= m_blocks.size() || index < 0)
	{
		return false;
	}
	else
	{
		return m_blocks[index].block.getType() == BlockType::Air;
	}
}

void Chunk::setChunkFaces()
{
	for (int z = 0; z < g_chunkSize.z; z++)
	{
		for (int y = 0; y < g_chunkSize.y; y++)
		{
			for (int x = 0; x < g_chunkSize.x; x++)
			{
				unsigned int currBlockIndex = 
					z * g_chunkSize.y * g_chunkSize.x + y * g_chunkSize.x + x;

				auto& currentBlock = m_blocks[currBlockIndex];
				
				if (currentBlock.block.getType() == BlockType::Air) 
				{ 
					continue; 
				}

				unsigned int frontBlock  = FRONT_FACE(x, y, z, g_chunkSize);
				unsigned int backBlock	 = BACK_FACE(x, y, z, g_chunkSize);
				unsigned int topBlock	 = TOP_FACE(x, y, z, g_chunkSize);
				unsigned int bottomBlock = BOTTOM_FACE(x, y, z, g_chunkSize);
				unsigned int rightBlock	 = RIGHT_FACE(x, y, z, g_chunkSize);
				unsigned int leftBlock	 = LEFT_FACE(x, y, z, g_chunkSize);

				bool isFrontFree  = checkAir(frontBlock)	|| (z == g_chunkSize.z - 1);
				bool isBackFree   = checkAir(backBlock)		|| (z == 0);
				bool isTopFree	  = checkAir(topBlock)		|| (y == g_chunkSize.y - 1);
				bool isBottomFree = checkAir(bottomBlock)	|| (y == 0) ;
				bool isRightFree  = checkAir(rightBlock)	|| (x == g_chunkSize.x - 1);
				bool isLeftFree   = checkAir(leftBlock)		|| (x == 0);

				if (isFrontFree)	{ currentBlock.front	= true; }
				if (isBackFree)		{ currentBlock.back		= true; }
				if (isTopFree)		{ currentBlock.top		= true; }
				if (isBottomFree)	{ currentBlock.bottom	= true; }
				if (isRightFree)	{ currentBlock.right	= true; }
				if (isLeftFree)		{ currentBlock.left		= true; }
			}
		}
	}
}

void Chunk::updateToNeighbourChunk(Chunk& chunk)
{
	float dx = m_pos.x - chunk.m_pos.x;
	float dz = m_pos.z - chunk.m_pos.z;

	if (dx > 0.0f)
	{
		unsigned int currentX = m_size.x - 1;
		unsigned int neighbourX = 0;
		traverseChunkFaceX(chunk, currentX, neighbourX);
	}
	else if (dx < 0.0f)
	{
		unsigned int currentX = 0;
		unsigned int neighbourX = chunk.m_size.x - 1;
		traverseChunkFaceX(chunk, currentX, neighbourX);
	}
	else if (dz > 0.0f)
	{
		unsigned int currentZ = m_size.z - 1;
		unsigned int neighbourZ = 0;
		traverseChunkFaceZ(chunk, currentZ, neighbourZ);
	}
	else if (dz < 0.0f)
	{
		unsigned int currentZ = 0;
		unsigned int neighbourZ = chunk.m_size.z - 1;
		traverseChunkFaceZ(chunk, currentZ, neighbourZ);
	}
}

void Chunk::traverseChunkFaceX(Chunk& chunk, const unsigned int currentX, const unsigned int neighbourX)
{
	for (unsigned int z = 0; z < m_size.z; z++)
	{
		for (unsigned int y = 0; y < m_size.y; y++)
		{
			unsigned int currentBlock	= z * m_size.x * m_size.y + y * g_chunkSize.x + currentX;
			unsigned int neighbourBlock = z * m_size.x * m_size.y + y * g_chunkSize.x + neighbourX;

			auto& currentChunkBlock = m_blocks[currentBlock];
			auto& neighbourChunkBlock = chunk.m_blocks[neighbourBlock];

			if (currentChunkBlock.block.getType() != BlockType::Air &&
				neighbourChunkBlock.block.getType() != BlockType::Air)
			{
				if (currentX > neighbourX)
				{
					currentChunkBlock.right = false;
					neighbourChunkBlock.left = false;
				}
				else
				{
					currentChunkBlock.left = false;
					neighbourChunkBlock.right = false;
				}
			}
		}
	}
}

void Chunk::traverseChunkFaceZ(Chunk& chunk, const unsigned int currentZ, const unsigned int neighbourZ)
{
	for (unsigned int y = 0; y < m_size.y; y++)
	{
		for (unsigned int x = 0; x < m_size.x; x++)
		{
			unsigned int currentBlock	= currentZ	 * m_size.x * m_size.y + y * g_chunkSize.x + x;
			unsigned int neighbourBlock = neighbourZ * m_size.x * m_size.y + y * g_chunkSize.x + x;

			if (m_blocks[currentBlock].block.getType()	!= BlockType::Air &&
				m_blocks[neighbourBlock].block.getType() != BlockType::Air)
			{
				auto& currentChunkBlock = m_blocks[currentBlock];
				auto& neighbourChunkBlock = chunk.m_blocks[neighbourBlock];

				if (currentChunkBlock.block.getType() != BlockType::Air &&
					neighbourChunkBlock.block.getType() != BlockType::Air)
				{
					if (currentZ > neighbourZ)
					{
						currentChunkBlock.front = false;
						neighbourChunkBlock.back = false;
					}
					else
					{
						currentChunkBlock.back = false;
						neighbourChunkBlock.front = false;
					}
				}
			}
		}
	}
}

void Chunk::setMesh()
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indicies;

	unsigned int IBOData_index = 0;
	for (auto& data : m_blocks)
	{
		bool shouldBeRendered =
			data.front || data.back ||
			data.top || data.bottom ||
			data.right || data.left;

		if (!shouldBeRendered)
		{
			continue;
		}

		if (data.front)		{ addFront(indicies, IBOData_index);	}
		if (data.back)		{ addBack(indicies, IBOData_index);		}
		if (data.top)		{ addTop(indicies, IBOData_index);		}
		if (data.bottom)	{ addBottom(indicies, IBOData_index);	}
		if (data.right)		{ addRight(indicies, IBOData_index);	}
		if (data.left)		{ addLeft(indicies, IBOData_index);		}

		vertices.insert(
			vertices.end(),
			data.block.getVertices().begin(),
			data.block.getVertices().end());

		IBOData_index++;
	}

	m_mesh = Mesh(vertices, indicies);
}

void Chunk::draw(Shader& shader, Texture& texture, glm::mat4 cameraView)
{
	m_mesh.draw(shader, texture, cameraView);
}
