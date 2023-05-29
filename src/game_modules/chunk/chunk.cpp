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

static constexpr unsigned char p[512] = {
	   151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142,
	   8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203,
	   117, 35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74,
	   165, 71, 134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220,
	   105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132,
	   187, 208, 89, 18, 169, 200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186,
	   3, 64, 52, 217, 226, 250, 124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59,
	   227, 47, 16, 58, 17, 182, 189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70,
	   221, 153, 101, 155, 167, 43, 172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178,
	   185, 112, 104, 218, 246, 97, 228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241,
	   81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176,
	   115, 121, 50, 45, 127, 4, 150, 254, 138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195,
	   78, 66, 215, 61, 156, 180,

	   151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142,
	   8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203,
	   117, 35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74,
	   165, 71, 134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220,
	   105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132,
	   187, 208, 89, 18, 169, 200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186,
	   3, 64, 52, 217, 226, 250, 124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59,
	   227, 47, 16, 58, 17, 182, 189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70,
	   221, 153, 101, 155, 167, 43, 172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178,
	   185, 112, 104, 218, 246, 97, 228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241,
	   81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176,
	   115, 121, 50, 45, 127, 4, 150, 254, 138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195,
	   78, 66, 215, 61, 156, 180,
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

Chunk::Chunk(glm::vec3 pos, int octaves, float persistance)
	: m_size(g_chunkSize)
	, m_pos(pos)
{
	// initGradientVectors();
	// check for nep();
	initBlocks(octaves, persistance);
	// setChunkFaces();
	// update neighbourFaces
}

float Chunk::perlin(float x, float y)
{
	auto interpolate = [](float a0, float a1, float w) {
		return (a1 - a0) * w + a0;
	};

	auto fade = [](float t) {
		return ((6 * t - 15) * t + 10) * t * t * t;
	};

	auto dot_grad = [](int hash, float xf, float yf) {
		// In 2D case, the gradient may be any of 8 direction vectors pointing to the
		// edges of a unit-square. The distance vector is the input offset (relative to
		// the smallest bound).
		switch (hash & 0x7)
		{
		case 0x0: return  xf + yf;
		case 0x1: return  xf;
		case 0x2: return  xf - yf;
		case 0x3: return -yf;
		case 0x4: return -xf - yf;
		case 0x5: return -xf;
		case 0x6: return -xf + yf;
		case 0x7: return  yf;
		default:  return  0.0f;
		}
	};

	// Top-left coordinates of the unit-square.
	const int xi0 = static_cast<int>(std::floor(x));
	const int yi0 = static_cast<int>(std::floor(y));

	// Input location in the unit-square.
	const float xf0 = x - xi0;
	const float yf0 = y - yi0;
	const float xf1 = xf0 - 1.0f;
	const float yf1 = yf0 - 1.0f;

	// Wrap to range 0-255.
	int const xi = xi0 & 0xFF;
	int const yi = yi0 & 0xFF;

	// Apply the fade function to the location.
	const float u = fade(xf0);
	const float v = fade(yf0);

	// Generate hash values for each point of the unit-square.
	int const h00 = p[p[(xi + 0)] + yi + 0];
	int const h01 = p[p[(xi + 0)] + yi + 1];
	int const h10 = p[p[(xi + 1)] + yi + 0];
	int const h11 = p[p[(xi + 1)] + yi + 1];

	// Linearly interpolate between dot products of each gradient with its distance to the input location.
	const float x1 = interpolate(dot_grad(h00, xf0, yf0), dot_grad(h10, xf1, yf0), u);
	const float x2 = interpolate(dot_grad(h01, xf0, yf1), dot_grad(h11, xf1, yf1), u);

	return (interpolate(x1, x2, v));
}

void Chunk::initBlocks(int octaves, float persistence)
{
	std::vector<int> heights;
	for (unsigned int z = 0; z < m_size.z; z++)
	{
		for (unsigned int x = 0; x < m_size.x; x++)
		{
			float frequency = 0.005f;
			float amplitude = 1.0f;

			float y = 0.0f;
			float maxValue = 0.0f;  // Used for normalizing result to 0.0 - 1.0
			for (int i = 0; i < octaves; i++)
			{
				y += (perlin((m_pos.x + x)  * frequency, (m_pos.z + z)  * frequency)) * amplitude;

				maxValue += amplitude;

				amplitude *= persistence;
				frequency *= 2;

			}
			heights.push_back(40 + 10 * octaves * (y/maxValue));
		}
	}


	m_blocks.reserve(m_size.x * m_size.y * m_size.z);
	for (unsigned int z = 0; z < m_size.z; z++)
	{
		for (unsigned int y = 0; y < m_size.y; y++)
		{
			for (unsigned int x = 0; x < m_size.x; x++)
			{
				glm::vec3 pos = { x, y, z };
				BlockType type = (y < heights[z * m_size.x + x] ? BlockType::Dirt : BlockType::Air);
				type = (y == heights[z * m_size.x + x] ? BlockType::GrassDirt : type);

				int index = z * m_size.x * m_size.y + y * m_size.x + x;
				m_blocks.emplace_back(m_pos + pos, type);
			}
		}
	}
}

bool Chunk::checkAir(int index)
{
	if (index >= m_blocks.size() || index < 0)
	{
		return false;
	}
	else
	{
		return m_blocks[index].getType() == BlockType::Air;
	}
}

#define FRONT_BLOCK(x, y, z, size)	(z + 1) * size.y * size.x + y * size.x + x
#define BACK_BLOCK(x, y, z, size)	(z - 1) * size.y * size.x + y * size.x + x
#define TOP_BLOCK(x, y, z, size)	z * size.y * size.x + (y + 1) * size.x + x
#define BOTTOM_BLOCK(x, y, z, size)	z * size.y * size.x + (y - 1) * size.x + x
#define RIGHT_BLOCK(x, y, z, size)	z * size.y * size.x + y * size.x + (x + 1)
#define LEFT_BLOCK(x, y, z, size)	z * size.y * size.x + y * size.x + (x - 1)

void Chunk::setChunkFaces()
{
	for (unsigned int z = 0; z < m_size.z; z++)
	{
		for (unsigned int y = 0; y < m_size.y; y++)
		{
			for (unsigned int x = 0; x < m_size.x; x++)
			{
				unsigned int currBlock =
					z * m_size.y * m_size.x + y * m_size.x + x;

				auto& currentBlock = m_blocks[currBlock];

				if (currentBlock.getType() == BlockType::Air)
				{
					continue;
				}

				unsigned int frontBlock = FRONT_BLOCK(x, y, z, m_size);
				unsigned int backBlock = BACK_BLOCK(x, y, z, m_size);
				unsigned int topBlock = TOP_BLOCK(x, y, z, m_size);
				unsigned int bottomBlock = BOTTOM_BLOCK(x, y, z, m_size);
				unsigned int rightBlock = RIGHT_BLOCK(x, y, z, m_size);
				unsigned int leftBlock = LEFT_BLOCK(x, y, z, m_size);

				currentBlock.front = checkAir(frontBlock);
				currentBlock.back = checkAir(backBlock);
				currentBlock.top = checkAir(topBlock);
				currentBlock.bottom = checkAir(bottomBlock);
				currentBlock.right = checkAir(rightBlock);
				currentBlock.left = checkAir(leftBlock);
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

			auto& leftBlock = (m_pos.x < chunk.m_pos.x) ? m_blocks[currentBlock] : chunk.m_blocks[neighbourBlock];
			auto& rightBlock = (m_pos.x > chunk.m_pos.x) ? m_blocks[currentBlock] : chunk.m_blocks[neighbourBlock];

			if (leftBlock.getType() != BlockType::Air && rightBlock.getType() != BlockType::Air)
			{
				leftBlock.right = false;
				rightBlock.left = false;
			}
			if (leftBlock.getType() == BlockType::Air && rightBlock.getType() != BlockType::Air)
			{
				rightBlock.left = true;
			}
			else if (leftBlock.getType() != BlockType::Air && rightBlock.getType() == BlockType::Air)
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

			auto& frontBlock = (m_pos.z > chunk.m_pos.z) ? m_blocks[currentBlock] : chunk.m_blocks[neighbourBlock];
			auto& backBlock = (m_pos.z < chunk.m_pos.z) ? m_blocks[currentBlock] : chunk.m_blocks[neighbourBlock];

			if (frontBlock.getType() != BlockType::Air && backBlock.getType() != BlockType::Air)
			{
				backBlock.front = false;
				frontBlock.back = false;
			}
			if (frontBlock.getType() == BlockType::Air && backBlock.getType() != BlockType::Air)
			{
				backBlock.front = true;
			}
			else if (frontBlock.getType() != BlockType::Air && backBlock.getType() == BlockType::Air)
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
	int iCurrBlock =
		currZ_index * m_size.x * m_size.y + currY_index * m_size.x + currX_index;

	// Find out in what direction ray should be traversed
	float tMaxX, tMaxY, tMaxZ;
	int stepX, stepY, stepZ;

	calcBlockBorderData(m_blocks[iCurrBlock], ray, tMaxX, tMaxY, tMaxZ, stepX, stepY, stepZ);

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

		iCurrBlock =
			currZ_index * m_size.x * m_size.y + currY_index * m_size.x + currX_index;
		if (iCurrBlock >= m_blocks.size())
		{
			return false;
		}
		auto& tmp = m_blocks[iCurrBlock];

		if (tmp.getType() != BlockType::Air)
		{
			tmp = Block(tmp.getPos(), BlockType::Air);
			checkSurroundedBlocks(currZ_index, currY_index, currX_index);
			return true;
		}

		calcBlockBorderData(tmp, ray, tMaxX, tMaxY, tMaxZ, stepX, stepY, stepZ);
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
		m_blocks[top].bottom = m_blocks[top].getType() != BlockType::Air ? true : false;
	}
	if (m_blocks.size() >= bottom)
	{
		m_blocks[bottom].top = m_blocks[bottom].getType() != BlockType::Air ? true : false;
	}
	if (m_blocks.size() >= front)
	{
		m_blocks[front].back = m_blocks[front].getType() != BlockType::Air ? true : false;
	}
	if (m_blocks.size() >= back)
	{
		m_blocks[back].front = m_blocks[back].getType() != BlockType::Air ? true : false;
	}
	if (m_blocks.size() >= left)
	{
		m_blocks[left].right = m_blocks[left].getType() != BlockType::Air ? true : false;
	}
	if (m_blocks.size() >= right)
	{
		m_blocks[right].left = m_blocks[right].getType() != BlockType::Air ? true : false;
	}

}

void Chunk::initMeshData()
{
	unsigned int IBOData_index = 0;

	m_vertices.clear();
	m_indicies.clear();

	for (auto& block : m_blocks)
	{
		bool shouldBeRendered =
			block.front || block.back ||
			block.top || block.bottom ||
			block.right || block.left;

		if (!shouldBeRendered)
		{
			continue;
		}

		if (block.front) { addFront(m_indicies, IBOData_index); }
		if (block.back) { addBack(m_indicies, IBOData_index); }
		if (block.top) { addTop(m_indicies, IBOData_index); }
		if (block.bottom) { addBottom(m_indicies, IBOData_index); }
		if (block.right) { addRight(m_indicies, IBOData_index); }
		if (block.left) { addLeft(m_indicies, IBOData_index); }

		addVertices(block);

		IBOData_index++;
	}
}

void Chunk::addVertices(Block& block)
{
	// front
	m_vertices.push_back({ block.getPos() + glm::vec3(-0.5f, -0.5f,  0.5f),  {0.0f, 0.0f, block.sideT_ind} });
	m_vertices.push_back({ block.getPos() + glm::vec3( 0.5f, -0.5f,  0.5f),  {1.0f, 0.0f,  block.sideT_ind} });
	m_vertices.push_back({ block.getPos() + glm::vec3(-0.5f,  0.5f,  0.5f),  {0.0f, 1.0f, block.sideT_ind} });
	m_vertices.push_back({ block.getPos() + glm::vec3( 0.5f,  0.5f,  0.5f),  {1.0f, 1.0f,  block.sideT_ind} });

	// back
	m_vertices.push_back({ block.getPos() + glm::vec3(-0.5f, -0.5f, -0.5f), {0.0f, 0.0f, block.sideT_ind} });
	m_vertices.push_back({ block.getPos() + glm::vec3( 0.5f, -0.5f, -0.5f),	{1.0f, 0.0f, block.sideT_ind} });
	m_vertices.push_back({ block.getPos() + glm::vec3(-0.5f,  0.5f, -0.5f),	{0.0f, 1.0f, block.sideT_ind} });
	m_vertices.push_back({ block.getPos() + glm::vec3( 0.5f,  0.5f, -0.5f),	{1.0f, 1.0f, block.sideT_ind} });

	// top
	m_vertices.push_back({ block.getPos() + glm::vec3(-0.5f, 0.5f,  0.5f),  {0.0f, 0.0f,  block.topT_ind} });
	m_vertices.push_back({ block.getPos() + glm::vec3( 0.5f, 0.5f,  0.5f),  {1.0f, 0.0f,  block.topT_ind} });
	m_vertices.push_back({ block.getPos() + glm::vec3(-0.5f, 0.5f, -0.5f),  {0.0f, 1.0f,  block.topT_ind} });
	m_vertices.push_back({ block.getPos() + glm::vec3( 0.5f, 0.5f, -0.5f),  {1.0f, 1.0f,  block.topT_ind} });

	// bottom
	m_vertices.push_back({ block.getPos() + glm::vec3(-0.5f, -0.5f,  0.5f), {0.0f, 0.0f,  block.bottomT_ind} });
	m_vertices.push_back({ block.getPos() + glm::vec3( 0.5f, -0.5f,  0.5f), {1.0f, 0.0f,  block.bottomT_ind} });
	m_vertices.push_back({ block.getPos() + glm::vec3(-0.5f, -0.5f, -0.5f), {0.0f, 1.0f,  block.bottomT_ind} });
	m_vertices.push_back({ block.getPos() + glm::vec3( 0.5f, -0.5f, -0.5f), {1.0f, 1.0f,  block.bottomT_ind} });

	// left
	m_vertices.push_back({ block.getPos() + glm::vec3(-0.5f, -0.5f, -0.5f), {0.0f, 0.0f, block.sideT_ind} });
	m_vertices.push_back({ block.getPos() + glm::vec3(-0.5f, -0.5f,  0.5f), {1.0f, 0.0f, block.sideT_ind} });
	m_vertices.push_back({ block.getPos() + glm::vec3(-0.5f,  0.5f, -0.5f), {0.0f, 1.0f, block.sideT_ind} });
	m_vertices.push_back({ block.getPos() + glm::vec3(-0.5f,  0.5f,  0.5f), {1.0f, 1.0f, block.sideT_ind} });

	// right
	m_vertices.push_back({ block.getPos() + glm::vec3(0.5f, -0.5f, -0.5f), {0.0f, 0.0f, block.sideT_ind} });
	m_vertices.push_back({ block.getPos() + glm::vec3(0.5f, -0.5f,  0.5f), {1.0f, 0.0f, block.sideT_ind} });
	m_vertices.push_back({ block.getPos() + glm::vec3(0.5f,  0.5f, -0.5f), {0.0f, 1.0f, block.sideT_ind} });
	m_vertices.push_back({ block.getPos() + glm::vec3(0.5f,  0.5f,  0.5f), {1.0f, 1.0f, block.sideT_ind} });
}

void Chunk::initMesh()
{
	m_meshInitialized = true;

	m_mesh = Mesh(m_vertices, m_blocks.size(), m_indicies, 6 * m_blocks.size());
}

void Chunk::setMesh()
{
	m_meshInitialized = true;

	m_mesh.updateData(m_vertices, m_indicies);
}

void Chunk::draw(Shader& shader, TextureArray& texture, glm::mat4 cameraView)
{
	m_mesh.draw(shader, texture, cameraView);
}
