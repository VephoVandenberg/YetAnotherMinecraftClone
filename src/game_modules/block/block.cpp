#include "../../engine/resource_manager/resource_manager.h"

#include "block.h"

using namespace Engine;
using namespace Data;
using namespace GameModules;

constexpr unsigned int g_numberOfFaces = 36;

constexpr float g_texX = 128.0f;
constexpr float g_texY = 128.0f;

Block::Block(glm::vec3 pos, BlockType type, glm::vec2 atlasSize)
	: m_pos(pos)
	, m_type(type)
{
	glm::vec2 topV1 = { 0.0f, 0.0f };
	glm::vec2 topV2 = { 0.0f, 0.0f };
	glm::vec2 topV3 = { 0.0f, 0.0f };
	glm::vec2 topV4 = { 0.0f, 0.0f };

	glm::vec2 bottomV1 = { 0.0f, 0.0f };
	glm::vec2 bottomV2 = { 0.0f, 0.0f };
	glm::vec2 bottomV3 = { 0.0f, 0.0f };
	glm::vec2 bottomV4 = { 0.0f, 0.0f };
	
	glm::vec2 sideV1 = { 0.0f, 0.0f };
	glm::vec2 sideV2 = { 0.0f, 0.0f };
	glm::vec2 sideV3 = { 0.0f, 0.0f };
	glm::vec2 sideV4 = { 0.0f, 0.0f };

	switch (m_type)
	{
	case BlockType::GrassDirt:
	{
		topV1 = { 0.0f,	(atlasSize.y - g_texY) / atlasSize.y };
		topV2 = { g_texX / atlasSize.x,	(atlasSize.y - g_texY) / atlasSize.y };
		topV3 = { 0.0f,	  1.0f};
		topV4 = { g_texX / atlasSize.x, 1.0f};

		bottomV1 = { g_texX / atlasSize.x, (atlasSize.y - g_texY) / atlasSize.y };
		bottomV2 = { (2.0f * g_texX) / atlasSize.x, (atlasSize.y - g_texY) / atlasSize.y };
		bottomV3 = { g_texX / atlasSize.x, 1.0f };
		bottomV4 = { (2.0f * g_texX) / atlasSize.x, 1.0f };

		sideV1 = { (2.0f * g_texX) / atlasSize.x, (atlasSize.y - g_texY) / atlasSize.y };
		sideV2 = { (3.0f * g_texX) / atlasSize.x, (atlasSize.y - g_texY) / atlasSize.y };
		sideV3 = { (2.0f * g_texX) / atlasSize.x, 1.0f };
		sideV4 = { (3.0f * g_texX) / atlasSize.x, 1.0f };
	}break;
	case BlockType::Dirt:
	{
		bottomV1 = { g_texX / atlasSize.x, (atlasSize.y - g_texY) / atlasSize.y };
		bottomV2 = { (2.0f * g_texX) / atlasSize.x, (atlasSize.y - g_texY) / atlasSize.y };
		bottomV3 = { g_texX / atlasSize.x, 1.0f };
		bottomV4 = { (2.0f * g_texX) / atlasSize.x, 1.0f };

		sideV1 = bottomV1;
		sideV2 = bottomV2;
		sideV3 = bottomV3;
		sideV4 = bottomV4;

		topV1 = bottomV1;
		topV2 = bottomV2;
		topV3 = bottomV3;
		topV4 = bottomV4;
	}break;
	case BlockType::Stone:
	{
		
	}break;

	case BlockType::Air:
		break;
	}

	m_vertices = {
		// front
		{ m_pos + glm::vec3(-0.5f, -0.5f,  0.5f), {sideV1 } },
		{ m_pos + glm::vec3(0.5f, -0.5f,  0.5f),  {sideV2 } },
		{ m_pos + glm::vec3(-0.5f,  0.5f,  0.5f), {sideV3 } },
		{ m_pos + glm::vec3(0.5f,  0.5f,  0.5f),  {sideV4 } },

		// back
		{ m_pos + glm::vec3(-0.5f, -0.5f, -0.5f), sideV1 },
		{ m_pos + glm::vec3( 0.5f, -0.5f, -0.5f), sideV2},
		{ m_pos + glm::vec3(-0.5f,  0.5f, -0.5f), sideV3 },
		{ m_pos + glm::vec3( 0.5f,  0.5f, -0.5f), sideV4 },

		// top
		{ m_pos + glm::vec3(-0.5f,  0.5f,  0.5f), topV1 },
		{ m_pos + glm::vec3( 0.5f,  0.5f,  0.5f), topV2 },
		{ m_pos + glm::vec3(-0.5f,  0.5f, -0.5f), topV3 },
		{ m_pos + glm::vec3( 0.5f,  0.5f, -0.5f), topV4 },

		// bottom
		{ m_pos + glm::vec3(-0.5f, -0.5f,  0.5f), bottomV1 },
		{ m_pos + glm::vec3( 0.5f, -0.5f,  0.5f), bottomV2 },
		{ m_pos + glm::vec3(-0.5f, -0.5f, -0.5f), bottomV3 },
		{ m_pos + glm::vec3( 0.5f, -0.5f, -0.5f), bottomV4 },

		// left
		{ m_pos + glm::vec3(-0.5f, -0.5f, -0.5f), sideV1 },
		{ m_pos + glm::vec3(-0.5f, -0.5f,  0.5f), sideV2 },
		{ m_pos + glm::vec3(-0.5f,  0.5f, -0.5f), sideV3 },
		{ m_pos + glm::vec3(-0.5f,  0.5f,  0.5f), sideV4 },

		// right									   
		{ m_pos + glm::vec3(0.5f, -0.5f, -0.5f), sideV1 },
		{ m_pos + glm::vec3(0.5f, -0.5f,  0.5f), sideV2 },
		{ m_pos + glm::vec3(0.5f,  0.5f, -0.5f), sideV3 },
		{ m_pos + glm::vec3(0.5f,  0.5f,  0.5f), sideV4 }
	};
}
