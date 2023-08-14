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

Chunk::Chunk(glm::vec3 pos)
	: m_size(g_chunkSize)
	, m_pos(pos)
{
	initBlocks();
}

float Chunk::perlin2D(float x, float y)
{
	auto interpolate = [](float a0, float a1, float w) {
		return (a1 - a0) * w + a0;
	};

	auto fade = [](float t) {
		return ((6 * t - 15) * t + 10) * t * t * t;
	};

	auto dot_grad = [](int hash, float x, float y) {
		// In 2D case, the gradient may be any of 8 direction vectors pointing to the
		// edges of a unit-square. The distance vector is the input offset (relative to
		// the smallest bound).
		switch (hash & 0x7)
		{
		case 0x0: return  x + y;
		case 0x1: return  x;
		case 0x2: return  x - y;
		case 0x3: return -y;
		case 0x4: return -x - y;
		case 0x5: return -x;
		case 0x6: return -x + y;
		case 0x7: return  y;
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
	const int xi = xi0 & 0xFF;
	const int yi = yi0 & 0xFF;

	// Apply the fade function to the location.
	const float u = fade(xf0);
	const float v = fade(yf0);

	// Generate hash values for each point of the unit-square.
	const int h00 = p[p[(xi + 0)] + yi + 0];
	const int h01 = p[p[(xi + 0)] + yi + 1];
	const int h10 = p[p[(xi + 1)] + yi + 0];
	const int h11 = p[p[(xi + 1)] + yi + 1];

	// Linearly interpolate between dot products of each gradient with its distance to the input location.
	const float x1 = interpolate(dot_grad(h00, xf0, yf0), dot_grad(h10, xf1, yf0), u);
	const float x2 = interpolate(dot_grad(h01, xf0, yf1), dot_grad(h11, xf1, yf1), u);

	return (interpolate(x1, x2, v));
}

float perlin3D(float x, float y, float z)
{
	auto lerp = [](float a0, float a1, float w) {
		return (a1 - a0) * w + a0;
	};

	auto fade = [](float t) {
		return ((6 * t - 15) * t + 10) * t * t * t;
	};

	auto dot_grad = [](int hash, float x, float y, float z) {
		// In 2D case, the gradient may be any of 8 direction vectors pointing to the
		// edges of a unit-square. The distance vector is the input offset (relative to
		// the smallest bound).
		switch (hash & 0xFF)
		{
		case 0x0: return  x + y;
		case 0x1: return -x + y;
		case 0x2: return  x - y;
		case 0x3: return -x - y;
		case 0x4: return  x + z;
		case 0x5: return -x + z;
		case 0x6: return  x - z;
		case 0x7: return -x - z;
		case 0x8: return  y + z;
		case 0x9: return -y + z;
		case 0xA: return  y - z;
		case 0xB: return -y - z;
		case 0xC: return  y + x;
		case 0xD: return -y + z;
		case 0xE: return  y - x;
		case 0xF: return -y - z;
		default: return 0.0f; // never happens I hope
		}
	};

	// Top-left coordinates of the unit-square.
	const int xi0 = static_cast<int>(std::floor(x));
	const int yi0 = static_cast<int>(std::floor(y));
	const int zi0 = static_cast<int>(std::floor(z));

	// Input location in the unit-square.
	const float xf0 = x - xi0;
	const float yf0 = y - yi0;
	const float zf0 = z - zi0;
	const float xf1 = xf0 - 1.0f;
	const float yf1 = yf0 - 1.0f;
	const float zf1 = zf0 - 1.0f;

	// Wrap to range 0-255.
	const int xi = xi0 & 0xFF;
	const int yi = yi0 & 0xFF;
	const int zi = zi0 & 0xFF;

	// Apply the fade function to the location.
	const float u = fade(xf0);
	const float v = fade(yf0);
	const float w = fade(zf0);

	// Generate hash values for each point of the unit-square.
	const int h000 = p[p[p[xi + 0] + yi + 0] + zi + 0];
	const int h001 = p[p[p[xi + 0] + yi + 0] + zi + 1];
	const int h010 = p[p[p[xi + 0] + yi + 1] + zi + 0];
	const int h011 = p[p[p[xi + 0] + yi + 1] + zi + 1];
	const int h100 = p[p[p[xi + 1] + yi + 0] + zi + 0];
	const int h101 = p[p[p[xi + 1] + yi + 0] + zi + 1];
	const int h110 = p[p[p[xi + 1] + yi + 1] + zi + 0];
	const int h111 = p[p[p[xi + 1] + yi + 1] + zi + 1];

	float x1, x2, y1, y2;
	
	x1 = lerp(dot_grad(h000, xf0, yf0, zf0), dot_grad(h100, xf1, yf0, zf0), u);
	x2 = lerp(dot_grad(h010, xf0, yf1, zf0), dot_grad(h110, xf1, yf1, zf0), u);
	y1 = lerp(x1, x2, v);
	
	x1 = lerp(dot_grad(h001, xf0, yf0, zf1), dot_grad(h101, xf1, yf0, zf1), u);
	x2 = lerp(dot_grad(h011, xf0, yf1, zf1), dot_grad(h111, xf1, yf1, zf1), u);
	y2 = lerp(x1, x2, v);

	return lerp(y1, y2, w);
}

void Chunk::initBlocks()
{
	for (unsigned int z = 0; z < m_size.z; z++)
	{
		for (unsigned int x = 0; x < m_size.x; x++)
		{

			int octaves = 14;
			float persistance = 0.5f;
			float frequency = 0.001f;
			float amplitude = 1.0f;
			float y = 0.0f;
			float maxValue = 0.0f;  // Used for normalizing result to 0.0 - 1.0
			for (int i = 0; i < octaves; i++)
			{
				y += (perlin2D((m_pos.x + x)  * frequency, (m_pos.z + z)  * frequency)) * amplitude;

				maxValue += amplitude;

				amplitude *= persistance;
				frequency *= 2;

			}
			m_heightMap.push_back(130 + 20 * octaves * (y/maxValue));
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
				BlockType type = getBlockType(m_heightMap[z * m_size.x + x], y);

				int index = z * m_size.x * m_size.y + y * m_size.x + x;
				m_blocks.emplace_back(m_pos + pos, type);
			}
		}
	}

	placeTrees();
}

// Still needs some work to be done
BlockType Chunk::getBlockType(int height, int index)
{
	constexpr int stoneBase = 50;
	constexpr int grassMountBase = 120;
	constexpr int snowBase = 140;

	if (index <= height)
	{
		if (height <= stoneBase || index < height - 10)
		{
			return BlockType::Stone;
		}

		if (index < height)
		{
			return BlockType::Dirt;
		}

		if (index == height)
		{
			return BlockType::GrassDirt;
		}
	}
	return BlockType::Air;
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
				const int iCurrent =
					z * m_size.y * m_size.x + y * m_size.x + x;

				if (m_blocks[iCurrent].getType() == BlockType::Air)
				{
					const int iFront = FRONT_BLOCK(x, y, z, m_size);
					if (z < m_size.z - 1 && 
						m_blocks[iFront].getType() != BlockType::Air)
					{
						m_blocks[iFront].back = true;
					}

					const int iTop = TOP_BLOCK(x, y, z, m_size);
					if (y < m_size.y - 1 &&
						m_blocks[iTop].getType() != BlockType::Air)
					{
						m_blocks[iTop].bottom = true;
					}

					const int iRight = RIGHT_BLOCK(x, y, z, m_size);
					if (x < m_size.x - 1 &&
						m_blocks[iRight].getType() != BlockType::Air)
					{
						m_blocks[iRight].left = true;
					}
				}
				else
				{
					const int iFront = FRONT_BLOCK(x, y, z, m_size);
					if (z < m_size.z - 1 &&
						m_blocks[iFront].getType() == BlockType::Air)
					{
						m_blocks[iCurrent].front = true;
					}

					const int iTop = TOP_BLOCK(x, y, z, m_size);
					if (y < m_size.y - 1 &&
						m_blocks[iTop].getType() == BlockType::Air)
					{
						m_blocks[iCurrent].top = true;
					}

					const int iRight = RIGHT_BLOCK(x, y, z, m_size);
					if (x < m_size.x - 1 &&
						m_blocks[iRight].getType() == BlockType::Air)
					{
						m_blocks[iCurrent].right = true;
					}
				}
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
			else if (leftBlock.getType() == BlockType::Air && rightBlock.getType() != BlockType::Air)
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

void Chunk::placeTrees()
{
	int numberOfTrees = rand() % 3;
	glm::vec3 treePos;

	for (int iTree = 0; iTree < numberOfTrees; iTree++)
	{
		int xPos = rand() % static_cast<int>(m_size.x);
		int zPos = rand() % static_cast<int>(m_size.z);
		int yPos = m_heightMap[zPos * m_size.x + xPos];
		treePos = {
			m_pos.x + xPos,
			m_pos.y + yPos + 1,
			m_pos.z + zPos
		};
		// Tree base creation
		for (int yBlock = 0; yBlock < 5; yBlock++)
		{
			m_blocks[zPos * m_size.x * m_size.y + treePos.y * m_size.x + xPos]
				= std::move(Block(treePos, BlockType::Wood));
			treePos.y += 1;
		}
		
		// Leaf creation;
		for (int z = 0; z < 3; z++)
		{
			for (int y = 0; y < 3; y++)
			{
				for (int x = 0; x < 3; x++)
				{
					glm::vec3 leafPos = {
						treePos.x + x - 1,
						treePos.y + y,
						treePos.z + z - 1
					};
					int iCurr = (zPos + z - 1) * m_size.x * m_size.y + leafPos.y * m_size.x + (xPos + x - 1);
					if (m_blocks.size() > iCurr)
					{
						m_blocks[iCurr] = std::move(Block(leafPos, BlockType::Leaf));
					}
					
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

			auto& frontBlock = (m_pos.z > chunk.m_pos.z) ? m_blocks[currentBlock] : chunk.m_blocks[neighbourBlock];
			auto& backBlock = (m_pos.z < chunk.m_pos.z) ? m_blocks[currentBlock] : chunk.m_blocks[neighbourBlock];

			if (frontBlock.getType() != BlockType::Air && backBlock.getType() != BlockType::Air)
			{
				backBlock.front = false;
				frontBlock.back = false;
			}
			else if (frontBlock.getType() == BlockType::Air && backBlock.getType() != BlockType::Air)
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

bool Chunk::processRayToRemoveBlock(const glm::vec3 traversePos)
{
	const int x = m_pos.x < 0 ? static_cast<int>(traversePos.x) - m_pos.x - 1 : static_cast<int>(traversePos.x) - m_pos.x;
	const int y = m_pos.y < 0 ? static_cast<int>(traversePos.y) - m_pos.y - 1 : static_cast<int>(traversePos.y) - m_pos.y;
	const int z = m_pos.z < 0 ? static_cast<int>(traversePos.z) - m_pos.z - 1 : static_cast<int>(traversePos.z) - m_pos.z;

	const int index = z * m_size.x * m_size.y + y * m_size.x + x;

	if (m_blocks[index].getType() != BlockType::Air)
	{
		m_blocks[index] = std::move(Block(m_blocks[index].getPos(), BlockType::Air));
		checkSurroundedBlocks(x, y, z);
		return true;
	}

	return false;
}

void Chunk::checkSurroundedBlocks(int x, int y, int z)
{
	const unsigned int back = BACK_BLOCK(x, y, z, m_size);
	const unsigned int top = TOP_BLOCK(x, y, z, m_size);
	const unsigned int bottom = BOTTOM_BLOCK(x, y, z, m_size);
	const unsigned int right = RIGHT_BLOCK(x, y, z, m_size);
	const unsigned int left = LEFT_BLOCK(x, y, z, m_size);
	const unsigned int front = FRONT_BLOCK(x, y, z, m_size);

	if (m_blocks.size() > top) { m_blocks[top].bottom = true; }
	if (m_blocks.size() > bottom) { m_blocks[bottom].top = true; }
	if (m_blocks.size() > front) { m_blocks[front].back = true; }
	if (m_blocks.size() > back) { m_blocks[back].front = true; }
	if (m_blocks.size() > left) { m_blocks[left].right= true; }
	if (m_blocks.size() > right) { m_blocks[right].left = true; }
}

void Chunk::initMeshData()
{
	unsigned int IBOData_index = 0;

	m_vertices.clear();

	for (auto& block : m_blocks)
	{
		if (block.getType() != BlockType::Air) { addVertices(block); }
	}
}

void Chunk::addVertices(Block& block)
{
	if (block.front)
	{
		m_vertices.push_back({ block.getPos() + glm::vec3(-0.5f, -0.5f,  0.5f),  {0.0f, 0.0f, block.sideT_ind}, {0.0f, 0.0f, 0.5f}, 0.6f });
		m_vertices.push_back({ block.getPos() + glm::vec3( 0.5f, -0.5f,  0.5f),  {1.0f, 0.0f, block.sideT_ind}, {0.0f, 0.0f, 0.5f}, 0.6f });
		m_vertices.push_back({ block.getPos() + glm::vec3(-0.5f,  0.5f,  0.5f),  {0.0f, 1.0f, block.sideT_ind}, {0.0f, 0.0f, 0.5f}, 0.6f });
		m_vertices.push_back({ block.getPos() + glm::vec3( 0.5f,  0.5f,  0.5f),  {1.0f, 1.0f, block.sideT_ind}, {0.0f, 0.0f, 0.5f}, 0.6f });
		m_vertices.push_back({ block.getPos() + glm::vec3(-0.5f,  0.5f,  0.5f),  {0.0f, 1.0f, block.sideT_ind}, {0.0f, 0.0f, 0.5f}, 0.6f });
		m_vertices.push_back({ block.getPos() + glm::vec3( 0.5f, -0.5f,  0.5f),  {1.0f, 0.0f, block.sideT_ind}, {0.0f, 0.0f, 0.5f}, 0.6f });

	}
	if (block.back)
	{
		m_vertices.push_back({ block.getPos() + glm::vec3(-0.5f, -0.5f, -0.5f), {0.0f, 0.0f, block.sideT_ind}, {0.0f, 0.0f, -0.5f}, 0.6f });
		m_vertices.push_back({ block.getPos() + glm::vec3(-0.5f,  0.5f, -0.5f), {0.0f, 1.0f, block.sideT_ind}, {0.0f, 0.0f, -0.5f}, 0.6f });
		m_vertices.push_back({ block.getPos() + glm::vec3( 0.5f, -0.5f, -0.5f), {1.0f, 0.0f, block.sideT_ind}, {0.0f, 0.0f, -0.5f}, 0.6f });
		m_vertices.push_back({ block.getPos() + glm::vec3(-0.5f,  0.5f, -0.5f), {0.0f, 1.0f, block.sideT_ind}, {0.0f, 0.0f, -0.5f}, 0.6f });
		m_vertices.push_back({ block.getPos() + glm::vec3( 0.5f,  0.5f, -0.5f), {1.0f, 1.0f, block.sideT_ind}, {0.0f, 0.0f, -0.5f}, 0.6f });
		m_vertices.push_back({ block.getPos() + glm::vec3( 0.5f, -0.5f, -0.5f), {1.0f, 0.0f, block.sideT_ind}, {0.0f, 0.0f, -0.5f}, 0.6f });
	}
	if (block.top)
	{
		m_vertices.push_back({ block.getPos() + glm::vec3(-0.5f, 0.5f,  0.5f),  {0.0f, 0.0f, block.topT_ind}, {0.0f, 0.5f, 0.0f}, 1.0f });
		m_vertices.push_back({ block.getPos() + glm::vec3( 0.5f, 0.5f,  0.5f),  {1.0f, 0.0f, block.topT_ind}, {0.0f, 0.5f, 0.0f}, 1.0f });
		m_vertices.push_back({ block.getPos() + glm::vec3(-0.5f, 0.5f, -0.5f),  {0.0f, 1.0f, block.topT_ind}, {0.0f, 0.5f, 0.0f}, 1.0f });
		m_vertices.push_back({ block.getPos() + glm::vec3( 0.5f, 0.5f,  0.5f),  {1.0f, 0.0f, block.topT_ind}, {0.0f, 0.5f, 0.0f}, 1.0f });
		m_vertices.push_back({ block.getPos() + glm::vec3( 0.5f, 0.5f, -0.5f),  {1.0f, 1.0f, block.topT_ind}, {0.0f, 0.5f, 0.0f}, 1.0f });
		m_vertices.push_back({ block.getPos() + glm::vec3(-0.5f, 0.5f, -0.5f),  {0.0f, 1.0f, block.topT_ind}, {0.0f, 0.5f, 0.0f}, 1.0f });
	}
	if (block.bottom)
	{
		m_vertices.push_back({ block.getPos() + glm::vec3(-0.5f, -0.5f,  0.5f), {0.0f, 0.0f, block.bottomT_ind}, {0.0f, -0.5f, 0.0f}, 0.3f });
		m_vertices.push_back({ block.getPos() + glm::vec3(-0.5f, -0.5f, -0.5f), {0.0f, 1.0f, block.bottomT_ind}, {0.0f, -0.5f, 0.0f}, 0.3f });
		m_vertices.push_back({ block.getPos() + glm::vec3( 0.5f, -0.5f,  0.5f), {1.0f, 0.0f, block.bottomT_ind}, {0.0f, -0.5f, 0.0f}, 0.3f });
		m_vertices.push_back({ block.getPos() + glm::vec3( 0.5f, -0.5f,  0.5f), {1.0f, 0.0f, block.bottomT_ind}, {0.0f, -0.5f, 0.0f}, 0.3f });
		m_vertices.push_back({ block.getPos() + glm::vec3(-0.5f, -0.5f, -0.5f), {0.0f, 1.0f, block.bottomT_ind}, {0.0f, -0.5f, 0.0f}, 0.3f });
		m_vertices.push_back({ block.getPos() + glm::vec3( 0.5f, -0.5f, -0.5f), {1.0f, 1.0f, block.bottomT_ind}, {0.0f, -0.5f, 0.0f}, 0.3f });
	}
	if (block.right)
	{
		m_vertices.push_back({ block.getPos() + glm::vec3(0.5f, -0.5f,  0.5f), {0.0f, 0.0f, block.sideT_ind}, {0.5f, 0.0f, 0.0f}, 0.35f });
		m_vertices.push_back({ block.getPos() + glm::vec3(0.5f, -0.5f, -0.5f), {1.0f, 0.0f, block.sideT_ind}, {0.5f, 0.0f, 0.0f}, 0.35f });
		m_vertices.push_back({ block.getPos() + glm::vec3(0.5f,  0.5f,  0.5f), {0.0f, 1.0f, block.sideT_ind}, {0.5f, 0.0f, 0.0f}, 0.35f });
		m_vertices.push_back({ block.getPos() + glm::vec3(0.5f, -0.5f, -0.5f), {1.0f, 0.0f, block.sideT_ind}, {0.5f, 0.0f, 0.0f}, 0.35f });
		m_vertices.push_back({ block.getPos() + glm::vec3(0.5f,  0.5f, -0.5f), {1.0f, 1.0f, block.sideT_ind}, {0.5f, 0.0f, 0.0f}, 0.35f });
		m_vertices.push_back({ block.getPos() + glm::vec3(0.5f,  0.5f,  0.5f), {0.0f, 1.0f, block.sideT_ind}, {0.5f, 0.0f, 0.0f}, 0.35f });
	}
	if (block.left)
	{
		m_vertices.push_back({ block.getPos() + glm::vec3(-0.5f, -0.5f, -0.5f), {0.0f, 0.0f, block.sideT_ind}, {-0.5f, 0.0f, 0.0f}, 0.85f });
		m_vertices.push_back({ block.getPos() + glm::vec3(-0.5f, -0.5f,  0.5f), {1.0f, 0.0f, block.sideT_ind}, {-0.5f, 0.0f, 0.0f}, 0.85f });
		m_vertices.push_back({ block.getPos() + glm::vec3(-0.5f,  0.5f, -0.5f), {0.0f, 1.0f, block.sideT_ind}, {-0.5f, 0.0f, 0.0f}, 0.85f });
		m_vertices.push_back({ block.getPos() + glm::vec3(-0.5f,  0.5f,  0.5f), {1.0f, 1.0f, block.sideT_ind}, {-0.5f, 0.0f, 0.0f}, 0.85f });
		m_vertices.push_back({ block.getPos() + glm::vec3(-0.5f,  0.5f, -0.5f), {0.0f, 1.0f, block.sideT_ind}, {-0.5f, 0.0f, 0.0f}, 0.85f });
		m_vertices.push_back({ block.getPos() + glm::vec3(-0.5f, -0.5f,  0.5f), {1.0f, 0.0f, block.sideT_ind}, {-0.5f, 0.0f, 0.0f}, 0.85f });
	}
}

void Chunk::initMesh()
{
	m_meshInitialized = true;

	m_mesh = Mesh(m_vertices);
}

void Chunk::setMesh()
{
	m_meshInitialized = true;

	m_mesh.updateData(m_vertices);
}

void Chunk::draw(Shader& shader, TextureArray& texture, glm::mat4 cameraView)
{
	m_mesh.draw(shader, texture, cameraView);
}
