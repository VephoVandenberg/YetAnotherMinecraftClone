#include <unordered_map>

#include "../../engine/shader/shader.h"
#include "../../engine/texture/texture.h"
#include "../../engine/ray/ray.h"

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
	for (unsigned int z = 0; z < g_chunkSize.z; z++)
	{
		for (unsigned int y = 0; y < g_chunkSize.y; y++)
		{
			for (unsigned int x = 0; x < g_chunkSize.x; x++)
			{
				glm::vec3 pos = m_pos + glm::vec3(x, y, z);
				BlockType type = (y < 20 ? BlockType::Dirt : BlockType::Air);
				type = (y == 20 ? BlockType::GrassDirt : type);

#if USE_VECTOR
				m_blocks.emplace_back(BlockRenderData(Block(pos, type)));
#else
#endif
			}
		}
	}
}

#define FRONT_BLOCK(x, y, z, size)	(z + 1) * size.y * size.x + y * size.x + x
#define BACK_BLOCK(x, y, z, size)	(z - 1) * size.y * size.x + y * size.x + x
#define TOP_BLOCK(x, y, z, size)		z * size.y * size.x + (y + 1) * size.x + x
#define BOTTOM_BLOCK(x, y, z, size)	z * size.y * size.x + (y - 1) * size.x + x
#define RIGHT_BLOCK(x, y, z, size)	z * size.y * size.x + y * size.x + (x + 1)
#define LEFT_BLOCK(x, y, z, size)	z * size.y * size.x + y * size.x + (x - 1)

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
	for (unsigned int z = 0; z < m_size.z; z++)
	{
		for (unsigned int y = 0; y < m_size.y; y++)
		{
			for (unsigned int x = 0; x < m_size.x; x++)
			{
				unsigned int currBlockIndex =
					z * m_size.y * m_size.x + y * m_size.x + x;

				auto& currentBlock = m_blocks[currBlockIndex];

				if (currentBlock.block.getType() == BlockType::Air)
				{
					continue;
				}

				unsigned int frontBlock = FRONT_BLOCK(x, y, z, m_size);
				unsigned int backBlock = BACK_BLOCK(x, y, z, m_size);
				unsigned int topBlock = TOP_BLOCK(x, y, z, m_size);
				unsigned int bottomBlock = BOTTOM_BLOCK(x, y, z, m_size);
				unsigned int rightBlock = RIGHT_BLOCK(x, y, z, m_size);
				unsigned int leftBlock = LEFT_BLOCK(x, y, z, m_size);

				bool isFrontFree = checkAir(frontBlock) || (z == m_size.z - 1);
				bool isBackFree = checkAir(backBlock) || (z == 0);
				bool isTopFree = checkAir(topBlock) || (y == m_size.y - 1);
				bool isBottomFree = checkAir(bottomBlock) || (y == 0);
				bool isRightFree = checkAir(rightBlock) || (x == m_size.x - 1);
				bool isLeftFree = checkAir(leftBlock) || (x == 0);

				if (isFrontFree) { currentBlock.front = true; }
				if (isBackFree) { currentBlock.back = true; }
				if (isTopFree) { currentBlock.top = true; }
				if (isBottomFree) { currentBlock.bottom = true; }
				if (isRightFree) { currentBlock.right = true; }
				if (isLeftFree) { currentBlock.left = true; }
			}
		}
	}
}

void Chunk::updateToNeighbourChunk(Chunk& chunk)
{
	if (m_pos.x > chunk.m_pos.x)
	{
		unsigned int currentX = 0;
		unsigned int neighbourX = chunk.m_size.x - 1;
		traverseChunkFaceX(chunk, currentX, neighbourX);
	}
	else if (m_pos.x < chunk.m_pos.x)
	{
		unsigned int currentX = m_size.x - 1;
		unsigned int neighbourX = 0;
		traverseChunkFaceX(chunk, currentX, neighbourX);
	}
	else if (m_pos.z > chunk.m_pos.z)
	{
		unsigned int currentZ = 0;
		unsigned int neighbourZ = chunk.m_size.z - 1;
		traverseChunkFaceZ(chunk, currentZ, neighbourZ);
	}
	else if (m_pos.z < chunk.m_pos.z)
	{
		unsigned int currentZ = m_size.z - 1;
		unsigned int neighbourZ = 0;
		traverseChunkFaceZ(chunk, currentZ, neighbourZ);
	}
}

void Chunk::traverseChunkFaceX(Chunk& chunk, const unsigned int currentX, const unsigned int neighbourX)
{
	for (unsigned int z = 0; z < m_size.z; z++)
	{
		for (unsigned int y = 0; y < m_size.y; y++)
		{
			unsigned int currentBlock
				= z * m_size.x * m_size.y + y * m_size.x + currentX;
			unsigned int neighbourBlock
				= z * m_size.x * m_size.y + y * m_size.x + neighbourX;

			auto& leftBlock = (currentX > neighbourX) ? m_blocks[currentBlock] : chunk.m_blocks[neighbourBlock];
			auto& rightBlock = (currentX < neighbourX) ? m_blocks[currentBlock] : chunk.m_blocks[neighbourBlock];

			if (leftBlock.block.getType() != BlockType::Air && rightBlock.block.getType() != BlockType::Air)
			{
				leftBlock.right = false;
				rightBlock.left = false;
			}
			else if (leftBlock.block.getType() == BlockType::Air && rightBlock.block.getType() != BlockType::Air)
			{
				rightBlock.left = true;
			}
			else if (leftBlock.block.getType() != BlockType::Air && rightBlock.block.getType() == BlockType::Air)
			{
				leftBlock.right = true;
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
			unsigned int currentBlock
				= currentZ * m_size.x * m_size.y + y * m_size.x + x;
			unsigned int neighbourBlock
				= neighbourZ * m_size.x * m_size.y + y * m_size.x + x;

			auto& frontBlock = (currentZ < neighbourZ) ? m_blocks[currentBlock] : chunk.m_blocks[neighbourBlock];
			auto& backBlock = (currentZ > neighbourZ) ? m_blocks[currentBlock] : chunk.m_blocks[neighbourBlock];
			
			if (frontBlock.block.getType() != BlockType::Air && backBlock.block.getType() != BlockType::Air)
			{
				frontBlock.back = false;
				backBlock.front = false;
			}
			else if (frontBlock.block.getType() == BlockType::Air && backBlock.block.getType() != BlockType::Air)
			{
				backBlock.front = true;
			}
			else if (frontBlock.block.getType() != BlockType::Air && backBlock.block.getType() == BlockType::Air)
			{
				frontBlock.back = true;
			}
		}
	}
}

void Chunk::calcBlockBorderData(const Block& block, const Ray& ray, float& tMaxX, float& tMaxY, float& tMaxZ, int& stepX, int& stepY, int& stepZ)
{
	float blockBorderX = block.getPos().x;
	if (ray.getDirection().x > 0.0f)
	{
		stepX = 1;
		blockBorderX += 1.0f;
		tMaxX = (blockBorderX - ray.getPosition().x) / ray.getDirection().x;
	}
	else  if (ray.getDirection().x < 0.0f)
	{
		stepX = -1;
		tMaxX = (blockBorderX - ray.getPosition().x) / ray.getDirection().x;
	}
	else
	{
		stepX = 0;
		tMaxX = 0.0f;
	}

	float blockBorderY = block.getPos().y;
	if (ray.getDirection().y > 0.0f)
	{
		stepY = 1;
		blockBorderY += 1.0f;
		tMaxY = (blockBorderY - ray.getPosition().y) / ray.getDirection().y;
	}
	else if (ray.getDirection().y < 0.0f)
	{
		stepY = -1;
		tMaxY = (blockBorderY - ray.getPosition().y) / ray.getDirection().y;
	}
	else
	{
		stepY = 0;
		tMaxY = 0.0f;
	}

	float blockBorderZ = block.getPos().z;
	if (ray.getDirection().z > 0.0f)
	{
		stepZ = 1;
		blockBorderZ += 1.0f;
		tMaxZ = (blockBorderZ - ray.getPosition().z) / ray.getDirection().z;
	}
	else if (ray.getDirection().z < 0.0f)
	{
		stepZ = -1;
		tMaxZ = (blockBorderZ - ray.getPosition().z) / ray.getDirection().z;
	}
	else
	{
		stepZ = 0;
		tMaxZ = 0.0f;
	}
}

bool Chunk::processRayToRemoveBlock(Ray& ray)
{
	int currX_index = ray.getPosition().x - m_pos.x;
	int endX_index = ray.getEndPoint().x - m_pos.x;
	int currY_index = ray.getPosition().y - m_pos.y;
	int endY_index = ray.getEndPoint().y - m_pos.y;
	int currZ_index = ray.getPosition().z - m_pos.z;
	int endZ_index = ray.getEndPoint().z - m_pos.z;

	// Find out where in chunk's block ray starts
	int iCurrBlock = currZ_index * m_size.x * m_size.y + currY_index * m_size.x + currX_index;

	// Find out in what direction ray should be traversed
	float tMaxX, tMaxY, tMaxZ;
	int stepX, stepY, stepZ;

	calcBlockBorderData(m_blocks[iCurrBlock].block, ray, tMaxX, tMaxY, tMaxZ, stepX, stepY, stepZ);

	while (
		currX_index < m_size.x && currX_index >= 0 && currX_index != endX_index &&
		currY_index < m_size.y && currY_index >= 0 && currY_index != endY_index &&
		currZ_index < m_size.z && currZ_index >= 0 && currZ_index != endZ_index)
	{
		if (tMaxX < tMaxY && tMaxX < tMaxZ)
		{
			currX_index += stepX;
		}
		else if (tMaxY < tMaxZ)
		{
			currY_index += stepY;
		}
		else
		{
			currZ_index += stepZ;
		}

		iCurrBlock = currZ_index * m_size.x * m_size.y + currY_index * m_size.x + currX_index;
		if (iCurrBlock >= m_blocks.size())
		{
			return false;
		}
		auto& tmp = m_blocks[iCurrBlock];

		if (tmp.block.getType() != BlockType::Air)
		{
			tmp = Block(tmp.block.getPos(), BlockType::Air);
			checkSurroundedBlocks(currZ_index, currY_index, currX_index);
			return true;
		}

		calcBlockBorderData(tmp.block, ray, tMaxX, tMaxY, tMaxZ, stepX, stepY, stepZ);
	}

	return false;
}

void Chunk::checkSurroundedBlocks(int z, int y, int x)
{
	unsigned int back = BACK_BLOCK(x, y, z, m_size);
	unsigned int top = TOP_BLOCK(x, y, z, m_size);
	unsigned int bottom = BOTTOM_BLOCK(x, y, z, m_size);
	unsigned int right = RIGHT_BLOCK(x, y, z, m_size);
	unsigned int left  = LEFT_BLOCK(x, y, z, m_size);
	unsigned int front = FRONT_BLOCK(x, y, z, m_size);


	if (m_blocks.size() >= top)
	{
		m_blocks[top].bottom = m_blocks[top].block.getType() != BlockType::Air ? true : false;
	}
	if (m_blocks.size() >= bottom)
	{
		m_blocks[bottom].top = m_blocks[bottom].block.getType() != BlockType::Air ? true : false;
	}
	if (m_blocks.size() >= front)
	{
		m_blocks[front].back = m_blocks[front].block.getType() != BlockType::Air ? true : false;
	}
	if (m_blocks.size() >= back)
	{
		m_blocks[back].front = m_blocks[back].block.getType() != BlockType::Air ? true : false;
	}
	if (m_blocks.size() >= left)
	{
		m_blocks[left].right = m_blocks[left].block.getType() != BlockType::Air ? true : false;
	}
	if (m_blocks.size() >= right)
	{
		m_blocks[right].left = m_blocks[right].block.getType() != BlockType::Air ? true : false;
	}

}

void Chunk::initMeshData(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
{
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

		if (data.front) { addFront(indices, IBOData_index); }
		if (data.back) { addBack(indices, IBOData_index); }
		if (data.top) { addTop(indices, IBOData_index); }
		if (data.bottom) { addBottom(indices, IBOData_index); }
		if (data.right) { addRight(indices, IBOData_index); }
		if (data.left) { addLeft(indices, IBOData_index); }

		vertices.insert(
			vertices.end(),
			data.block.getVertices().begin(),
			data.block.getVertices().end());

		IBOData_index++;
	}

}

void Chunk::initMesh()
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	initMeshData(vertices, indices);

	m_mesh = Mesh(vertices, m_blocks.size(), indices, 6 * m_blocks.size());
}

void Chunk::setMesh()
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	initMeshData(vertices, indices);

	m_mesh.updateData(vertices, indices);
}

void Chunk::draw(Shader& shader, TextureArray& texture, glm::mat4 cameraView)
{
	m_mesh.draw(shader, texture, cameraView);
}
