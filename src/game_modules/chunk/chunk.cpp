#include "../../engine/shader/shader.h"
#include "../../engine/texture/texture.h"

#include "../block/block.h"

#include "chunk.h"

using namespace GameModules;

constexpr glm::vec3 g_chunkSize = { 16.0f, 256.0f, 16.0f };

Chunk::Chunk(glm::vec3 pos)
	: m_size(g_chunkSize)
	, m_pos(pos)
{
	initBlocks();
	setChunkFaces();
}

void Chunk::initBlocks()
{
	for (int z = 0; z < g_chunkSize.z; z++)
	{
		for (int y = 0; y < g_chunkSize.y; y++)
		{
			for (int x = 0; x < g_chunkSize.x; x++)
			{
				glm::vec3 pos = { x, y, z };
				BlockType type = (y < 50 ? BlockType::Grass : BlockType::Air);

				m_blocks.emplace_back(Block(pos, type));
			}
		}
	}
}

void Chunk::setChunkFaces()
{
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

	int IBOData_index = 0;
	for (int z = 0; z < g_chunkSize.z; z++)
	{
		for (int y = 0; y < g_chunkSize.y; y++)
		{
			for (int x = 0; x < g_chunkSize.x; x++)
			{
				unsigned int currBlockIndex = z * g_chunkSize.y * g_chunkSize.x + y * g_chunkSize.x + x;

				auto& currentBlock = m_blocks[currBlockIndex];
				/*
				if (currentBlock.getType() == BlockType::Air) 
				{ 
					continue; 
				}
				*/

				unsigned int frontBlock	 = (z + 1) * g_chunkSize.y * g_chunkSize.x + y * g_chunkSize.x + x;
				unsigned int backBlock	 = (z - 1) * g_chunkSize.y * g_chunkSize.x + y * g_chunkSize.x + x;
				unsigned int topBlock	 = z * g_chunkSize.y * g_chunkSize.x + (y + 1) * g_chunkSize.x + x;
				unsigned int bottomBlcok = z * g_chunkSize.y * g_chunkSize.x + (y - 1) * g_chunkSize.x + x;
				unsigned int rightBlock	 = z * g_chunkSize.y * g_chunkSize.x + y * g_chunkSize.x + (x + 1);
				unsigned int leftBlock	 = z * g_chunkSize.y * g_chunkSize.x + y * g_chunkSize.x + (x - 1);

				bool isFrontFree	= (z == g_chunkSize.z - 1);
				bool isBackFree		= (z == 0);
				bool isTopFree		= (y == g_chunkSize.y - 1);	
				bool isBottomFree	= (y == 0);
				bool isRightFree	= (x == g_chunkSize.x - 1);
				bool isLeftFree		= (x == 0);

				if (isFrontFree)	{ addFront(m_indicies,  IBOData_index);	}
				if (isBackFree)		{ addBack(m_indicies,	IBOData_index);	}
				if (isTopFree)		{ addTop(m_indicies,	IBOData_index);	}
				if (isBottomFree)	{ addBottom(m_indicies,	IBOData_index);	}
				if (isRightFree)	{ addRight(m_indicies,	IBOData_index);	}
				if (isLeftFree)		{ addLeft(m_indicies,	IBOData_index);	}

				IBOData_index++;

				m_vertices.insert(
					m_vertices.end(),
					m_blocks[currBlockIndex].getVertices().begin(),
					m_blocks[currBlockIndex].getVertices().end());
			}
		}
	}

	m_mesh = std::move(Mesh(m_vertices, m_indicies));
}

void Chunk::update()
{

}

void Chunk::draw(Shader& shader, Texture& texture, glm::mat4 cameraView)
{
	m_mesh.draw(shader, texture, cameraView);
}
