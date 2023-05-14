#include <glm/gtc/random.hpp>

#ifdef DEBUG_PERLIN
#include <iostream>
#endif

#include "../../engine/shader/shader.h"
#include "../../engine/texture/texture.h"
#include "../../engine/ray/ray.h"

#include "../block/block.h"

#include "chunk.h"

using namespace GameModules;

constexpr glm::vec3 g_chunkSize = { 16.0f, 256.0f, 16.0f };
constexpr static glm::vec2 g_gradients[] = {
	{-1, -1},
	{ 1, -1},
	{-1,  1},
	{ 1,  1}
};

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

Chunk::Chunk(glm::vec3 pos, const std::vector<int>& heightMap)
	: m_size(g_chunkSize)
	, m_pos(pos)
	, m_heightMap(heightMap)
{
	//initGradientVectors();
	// Check for neighbourVectors
	//initHeightMap();
	//initBlocks();
	//setChunkFaces();
	// update neighbourFaces
}

void Chunk::initGradientVectors()
{
	m_gradients.reserve((m_size.x / 4 + 1) * (m_size.z / 4 + 1));
	for (unsigned int z = 0; z < m_size.z / 4 + 1; z++)
	{
		for (unsigned int x = 0; x < m_size.x / 4 + 1; x++)
		{
			// Calculate gradient vectors
			glm::vec2 randVec = {
				glm::linearRand(-rand(), rand()),
				glm::linearRand(-rand(), rand())
			};
			glm::vec2 pos = { m_pos.x + x, m_pos.z + z };
			glm::vec2 gradient = glm::normalize(randVec);

			m_gradients.push_back(gradient);
		}
	}
}

void Chunk::initHeightMap()
{
	for (unsigned int z = 0; z < m_size.z; z++)
	{
		for (unsigned int x = 0; x < m_size.x; x++)
		{
			m_heightMap.push_back(perlin1(x, z));
		}
	}
}

int Chunk::perlin1(int x, int z)
{
	auto interpolate = [](float a0, float a1, float w) {
		return (a1 - a0) * (3.0 - w * 2.0) * w * w + a0;
	};

	auto fade = [](float t) {
		return ((6 * t - 15) * t + 10) * t * t * t;
	};

	unsigned int bottomL = z / (m_size.z / 4) * (m_size.x / 4) + x / (m_size.x / 4);
	unsigned int bottomR = z / (m_size.z / 4) * (m_size.x / 4) + x / (m_size.x / 4) + 1;
	unsigned int topL = (z / (m_size.z / 4) + 1) * (m_size.x / 4) + x / (m_size.x / 4);
	unsigned int topR = (z / (m_size.z / 4) + 1) * (m_size.x / 4) + x / (m_size.x / 4) + 1;

	int xRand = rand() % (int)(m_size.x - 1) + 0;
	int zRand = rand() % (int)(m_size.z - 1) + 0;

	glm::vec2 randomPointInQuad = {
		x + static_cast<float>(rand()) / static_cast<float>(RAND_MAX),
		z + static_cast<float>(rand()) / static_cast<float>(RAND_MAX)
	};

	glm::vec2 dBottomL = randomPointInQuad - glm::vec2(x / (m_size.x / 4), z / (m_size.z / 4));
	glm::vec2 dBottomR = randomPointInQuad - glm::vec2(x / (m_size.x / 4) + m_size.x / 4, z / (m_size.z / 4));
	glm::vec2 dTopL = randomPointInQuad -	 glm::vec2(x / (m_size.x / 4), z / (m_size.z / 4) + m_size.z / 4);
	glm::vec2 dTopR = randomPointInQuad -	 glm::vec2(x / (m_size.x / 4) + m_size.x / 4, z / (m_size.z / 4) + m_size.z / 4);

	float dPrTopL = glm::dot(dTopL, m_gradients[topL]);
	float dPrTopR = glm::dot(dTopR, m_gradients[topR]);
	float dPrBottomL = glm::dot(dBottomL, m_gradients[bottomL]);
	float dPrBottomR = glm::dot(dBottomR, m_gradients[bottomR]);

	float sx = randomPointInQuad.x - static_cast<float>(x);
	float sz = randomPointInQuad.y - static_cast<float>(z);

	float value = interpolate(
		interpolate(dPrTopL, dPrTopR, fade(sx)),
		interpolate(dPrBottomL, dPrBottomR, fade(sx)),
		fade(sz));

#ifdef DEBUG_PERLIN
	// std::cout << value << std::endl;
#endif

	if (value < 0)
	{
		return 0;
	}
	return value / 3;
}

// That's JavidX9 code try to reuse it tomorrow
int Chunk::perlin2(int nOctaves, float fBias)
{
	std::vector<float> seed;
	for (unsigned int z = 0; z < g_chunkSize.z * 2; z++)
	{
		for (unsigned int x = 0; x < g_chunkSize.x * 2; x++)
		{
			seed.push_back((float)rand() / (float)RAND_MAX);
		}
	}


	// Used 1D Perlin Noise
	for (int x = 0; x < m_size.x; x++)
	{
		for (int z = 0; z < m_size.z; z++)
		{
			float fNoise = 0.0f;
			float fScaleAcc = 0.0f;
			float fScale = 1.0f;

			for (int o = 0; o < nOctaves; o++)
			{
				int nPitch = static_cast<int>(m_size.x) >> o;
				int nSampleX1 = (x / nPitch) * nPitch;
				int nSampleZ1 = (z / nPitch) * nPitch;

				int nSampleX2 = (nSampleX1 + nPitch) % static_cast<int>(m_size.x);
				int nSampleZ2 = (nSampleZ1 + nPitch) % static_cast<int>(m_size.x);

				float fBlendX = (float)(x - nSampleX1) / (float)nPitch;
				float fBlendZ = (float)(z - nSampleZ1) / (float)nPitch;

				float fSampleT = (1.0f - fBlendX) * seed[nSampleZ1 * static_cast<int>(m_size.x) + nSampleX1] + fBlendX * seed[nSampleZ1 * static_cast<int>(m_size.x) + nSampleX2];
				float fSampleB = (1.0f - fBlendX) * seed[nSampleZ2 * static_cast<int>(m_size.x) + nSampleX1] + fBlendX * seed[nSampleZ2 * static_cast<int>(m_size.x) + nSampleX2];

				fScaleAcc += fScale;
				fNoise += (fBlendZ * (fSampleB - fSampleT) + fSampleT) * fScale;
				fScale = fScale / fBias;
			}

			// Scale to seed range
			m_heightMap.push_back(3 * fNoise / fScaleAcc);
		}
	}
	return 0;
}

void Chunk::updateGradientsToNieghbouChunk(const Chunk& chunk)
{
	if (m_pos.x > chunk.m_pos.x)
	{
		traverseChunkGradX(chunk, 0, chunk.m_size.x - 1);
	}
	else if (m_pos.x < chunk.m_pos.x)
	{
		traverseChunkGradX(chunk, chunk.m_size.x - 1, 0);
	}
	else if (m_pos.z > chunk.m_pos.z)
	{
		traverseChunkGradZ(chunk, 0, chunk.m_size.z - 1);
	}
	else if (m_pos.z < chunk.m_pos.z)
	{
		traverseChunkGradZ(chunk, chunk.m_size.z - 1, 0);
	}
}

void Chunk::traverseChunkGradX(const Chunk& chunk, const unsigned int currentX, const unsigned int neighbourX)
{
	for (unsigned int z = 0; z < m_size.z / 4 + 1; z++)
	{
		//m_gradients[z / (m_size.z / 4) * m_size.x / 4 + currentX / (m_size.x / 4)] =
		//	chunk.m_gradients[z / (m_size.z / 4) * m_size.x / 4 + neighbourX / (m_size.x / 4)];
	}
}

void Chunk::traverseChunkGradZ(const Chunk& chunk, const unsigned int currentZ, const unsigned int neighbourZ)
{
	for (unsigned int x = 0; x < m_size.x / 4 + 1; x++)
	{
		//m_gradients[currentZ / (m_size.z / 4) * m_size.x / 4 + x / (m_size.x / 4)] =
		//	chunk.m_gradients[neighbourZ / (m_size.z / 4) * m_size.x / 4 + x / (m_size.x / 4)];
	}
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
				BlockType type = (y < 20 + m_heightMap[z * m_size.z + x] ? BlockType::Dirt : BlockType::Air);
				type = (y == 20 + m_heightMap[z * m_size.z + x] ? BlockType::GrassDirt : type);

#if USE_VECTOR_FOR_BLOCKS
				m_blocks.emplace_back(BlockRenderData(Block(pos, type)));
#else
#endif
			}
		}
	}
	}

#define FRONT_BLOCK(x, y, z, size)	(z + 1) * size.y * size.x + y * size.x + x
#define BACK_BLOCK(x, y, z, size)	(z - 1) * size.y * size.x + y * size.x + x
#define TOP_BLOCK(x, y, z, size)	z * size.y * size.x + (y + 1) * size.x + x
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
	glm::vec3 dir = ray.getEndPoint() - ray.getPosition();
	if (ray.getDirection().x > 0.0f)
	{
		stepX = 1;
		blockBorderX += 1.0f;
		tMaxX = (blockBorderX - ray.getPosition().x) / dir.x;
	}
	else  if (ray.getDirection().x < 0.0f)
	{
		stepX = -1;
		tMaxX = (blockBorderX - ray.getPosition().x) / dir.x;
	}

	float blockBorderY = block.getPos().y;
	if (ray.getDirection().y > 0.0f)
	{
		stepY = 1;
		blockBorderY += 1.0f;
		tMaxY = (blockBorderY - ray.getPosition().y) / dir.y;
	}
	else if (ray.getDirection().y < 0.0f)
	{
		stepY = -1;
		tMaxY = (blockBorderY - ray.getPosition().y) / dir.y;
	}

	float blockBorderZ = block.getPos().z;
	if (ray.getDirection().z > 0.0f)
	{
		stepZ = 1;
		blockBorderZ += 1.0f;
		tMaxZ = (blockBorderZ - ray.getPosition().z) / dir.z;
	}
	else if (ray.getDirection().z < 0.0f)
	{
		stepZ = -1;
		tMaxZ = (blockBorderZ - ray.getPosition().z) / dir.z;
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
	unsigned int left = LEFT_BLOCK(x, y, z, m_size);
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
