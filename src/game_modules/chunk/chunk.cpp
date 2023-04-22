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

#if USE_VECTOR
#define FRONT_FACE(x, y, z, size)	(z + 1) * size.y * size.x + y * size.x + x
#define BACK_FACE(x, y, z, size)	(z - 1) * size.y * size.x + y * size.x + x
#define TOP_FACE(x, y, z, size)		z * size.y * size.x + (y + 1) * size.x + x
#define BOTTOM_FACE(x, y, z, size)	z * size.y * size.x + (y - 1) * size.x + x
#define RIGHT_FACE(x, y, z, size)	z * size.y * size.x + y * size.x + (x + 1)
#define LEFT_FACE(x, y, z, size)	z * size.y * size.x + y * size.x + (x - 1)
#else
#endif

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

				unsigned int frontBlock = FRONT_FACE(x, y, z, m_size);
				unsigned int backBlock = BACK_FACE(x, y, z, m_size);
				unsigned int topBlock = TOP_FACE(x, y, z, m_size);
				unsigned int bottomBlock = BOTTOM_FACE(x, y, z, m_size);
				unsigned int rightBlock = RIGHT_FACE(x, y, z, m_size);
				unsigned int leftBlock = LEFT_FACE(x, y, z, m_size);

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
		unsigned int currentX = chunk.m_size.x - 1;
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

			auto& currentChunkBlock = m_blocks[currentBlock];
			auto& neighbourChunkBlock = chunk.m_blocks[neighbourBlock];

			if (currentChunkBlock.block.getType() != BlockType::Air &&
				neighbourChunkBlock.block.getType() != BlockType::Air)
			{
				if (currentX < neighbourX)
				{
					currentChunkBlock.left = false;
					neighbourChunkBlock.right = false;
				}
				else
				{
					currentChunkBlock.right = false;
					neighbourChunkBlock.left = false;
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
			unsigned int currentBlock
				= currentZ * m_size.x * m_size.y + y * m_size.x + x;
			unsigned int neighbourBlock
				= neighbourZ * m_size.x * m_size.y + y * m_size.x + x;

			if (m_blocks[currentBlock].block.getType() != BlockType::Air &&
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

bool Chunk::rayChunkIntersection(const Ray& ray, Block& block, glm::vec3& t0, glm::vec3& t1)
{
	const float invDirX = 1.0f / ray.getDirection().x;
	if (invDirX >= 0.0f)
	{
		t0.x = (block.getPos().x - ray.getDirection().x) * invDirX;
		t1.x = (block.getPos().x + 1.0f - ray.getDirection().x) * invDirX;
	}
	else
	{
		t1.x = (block.getPos().x - ray.getDirection().x) * invDirX;
		t0.x = (block.getPos().x + 1.0f - ray.getDirection().x) * invDirX;
	}

	const float invDirY = 1.0f / ray.getDirection().y;
	if (invDirY >= 0.0f)
	{
		t0.y = (block.getPos().y - ray.getDirection().y) * invDirY;
		t1.y = (block.getPos().y + 1.0f - ray.getDirection().y) * invDirY;
	}
	else
	{
		t1.y = (block.getPos().y - ray.getDirection().y) * invDirY;
		t0.y = (block.getPos().y + 1.0f - ray.getDirection().y) * invDirY;
	}

	if (t0.x > t1.y || t0.y > t1.x) { return false; }
	if (t0.y > t0.x) { t0.x = t0.y; }
	if (t1.y < t1.x) { t1.x = t1.y; }

	const float invDirZ = 1.0f / ray.getDirection().z;
	if (invDirZ >= 0.0f)
	{
		t0.z = (m_pos.z - ray.getDirection().z) * invDirZ;
		t1.z = (m_pos.z + 1.0f - ray.getDirection().z) * invDirZ;
	}
	else
	{
		t1.z = (m_pos.z - ray.getDirection().z) * invDirZ;
		t0.z = (m_pos.z + 1.0f - ray.getDirection().z) * invDirZ;
	}

	if (t0.x > t1.z || t0.z > t1.x) { return false; }
	if (t0.z > t0.x) { t0.x = t0.z; }
	if (t1.z < t1.x) { t1.x = t1.z; }

	return true;
}

bool Chunk::processRayToRemoveBlock(Ray& ray)
{
	int currX_index = ray.getPosition().x - m_pos.x;
	int endX_index	= ray.getEndPoint().x - m_pos.x;
	int currY_index = ray.getPosition().y - m_pos.y;
	int endY_index	= ray.getEndPoint().y - m_pos.y;
	int currZ_index = ray.getPosition().z - m_pos.z;
	int endZ_index	= ray.getEndPoint().z - m_pos.z;

	glm::vec3 t0, t1;

	unsigned int currIndex = currZ_index * m_size.x * m_size.y + currY_index * m_size.x + currX_index;
	rayChunkIntersection(ray, m_blocks[currIndex].block, t0, t1);

	int stepX;
	float tDeltaX;
	float tMaxX;
	if (ray.getDirection().x > 0.0f)
	{
		stepX = 1;
		tDeltaX = 1 / ray.getDirection().x;
		tMaxX = t0.x + (m_pos.x + currX_index - ray.getPosition().x) / ray.getDirection().x;
	}
	else if (ray.getDirection().x < 0.0f)
	{
		stepX = -1;
		tDeltaX = 1 / ray.getDirection().x;
		tMaxX = t0.x + (m_pos.x + currX_index - 1 - ray.getPosition().x) / ray.getDirection().x;
	}
	else
	{
		stepX = 0;
		tDeltaX = t1.x;
		tMaxX = t1.x;
	}

	int stepY;
	float tDeltaY;
	float tMaxY;
	if (ray.getDirection().y > 0.0f)
	{
		stepY = 1;
		tDeltaY = 1 / ray.getDirection().y;
		tMaxY = t0.x + (m_pos.y + currY_index - ray.getPosition().y) / ray.getDirection().y;
	}
	else if (ray.getDirection().y < 0.0f)
	{
		stepY = -1;
		tDeltaY = 1 / ray.getDirection().y;
		tMaxY = t0.x + (m_pos.y + currY_index - 1 - ray.getPosition().y) / ray.getDirection().y;
	}
	else
	{
		stepY = 0;
		tDeltaY = t1.x;
		tMaxY = t1.x;
	}

	int stepZ;
	float tDeltaZ;
	float tMaxZ;
	if (ray.getDirection().z > 0.0f)
	{
		stepZ = 1;
		tDeltaZ = 1 / ray.getDirection().z;
		tMaxZ = t0.x + (m_pos.z + currZ_index - ray.getPosition().y) / ray.getDirection().z;
	}
	else if (ray.getDirection().z < 0.0f)
	{
		stepZ = -1;
		tDeltaZ = 1 / ray.getDirection().z;
		tMaxZ = t0.x + (m_pos.z + currZ_index - 1 - ray.getPosition().y) / ray.getDirection().z;
	}
	else
	{
		stepZ = 0;
		tDeltaZ = t1.x;
		tMaxZ = t1.x;
	}

	while (currX_index != endX_index || currY_index != endY_index || currZ_index != endZ_index)
	{
		if (m_blocks[currIndex].block.getType() != BlockType::Air)
		{
			m_blocks[currIndex] = BlockRenderData(Block(m_blocks[currIndex].block.getPos(), BlockType::Air));
			return true;
		}

		if (tMaxX < tMaxY && tMaxX < tMaxZ)
		{
			currX_index += stepX;
			tMaxX += tDeltaX;
		}
		else if (tMaxY < tMaxZ)
		{
			currY_index += stepY;
			tMaxY += tDeltaY;
		}
		else
		{
			currZ_index += stepZ;
			tMaxZ += tDeltaZ;
		}

		currIndex = currZ_index * m_size.x * m_size.y + currY_index * m_size.x + currX_index;

	}

	return false;
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
