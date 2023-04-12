#include "../../engine/resource_manager/resource_manager.h"

#include "block.h"

using namespace Engine;
using namespace Data;
using namespace GameModules;

constexpr unsigned int g_numberOfFaces = 36;

constexpr float g_texX = 128.0f;
constexpr float g_texY = 128.0f;

Block::Block(glm::vec3 pos, BlockType type)
	: m_pos(pos)
	, m_type(type)
{
	float top = 0.0f;
	float bottom = 0.0f;
	float side = 0.0f;

	switch (m_type)
	{
	case BlockType::GrassDirt:
	{
		top = 2.0f;
		bottom = 1.0f;
		side = 0.0f;
	}break;

	case BlockType::Dirt:
	{
		top = bottom = side = 1.0f;
	}break;

	case BlockType::Stone:
	{

	}break;

	case BlockType::Air:
	{}break;
	}

	m_vertices = {
		// front
		{ m_pos + glm::vec3(-0.5f, -0.5f,  0.5f),  {0.0f, 0.0f, side}},
		{ m_pos + glm::vec3( 0.5f, -0.5f,  0.5f),  {1.0f, 0.0f, side}},
		{ m_pos + glm::vec3(-0.5f,  0.5f,  0.5f),  {0.0f, 1.0f, side}},
		{ m_pos + glm::vec3( 0.5f,  0.5f,  0.5f),  {1.0f, 1.0f, side}},

		// back
		{ m_pos + glm::vec3(-0.5f, -0.5f, -0.5f), {0.0f, 0.0f, side}},
		{ m_pos + glm::vec3( 0.5f, -0.5f, -0.5f), {1.0f, 0.0f, side}},
		{ m_pos + glm::vec3(-0.5f,  0.5f, -0.5f), {0.0f, 1.0f, side}},
		{ m_pos + glm::vec3( 0.5f,  0.5f, -0.5f), {1.0f, 1.0f, side}},

		// top
		{ m_pos + glm::vec3(-0.5f,  0.5f,  0.5f), {0.0f, 0.0f, top}},
		{ m_pos + glm::vec3( 0.5f,  0.5f,  0.5f), {1.0f, 0.0f, top}},
		{ m_pos + glm::vec3(-0.5f,  0.5f, -0.5f), {0.0f, 1.0f, top}},
		{ m_pos + glm::vec3( 0.5f,  0.5f, -0.5f), {1.0f, 1.0f, top}},

		// bottom
		{ m_pos + glm::vec3(-0.5f, -0.5f,  0.5f), {0.0f, 0.0f, bottom}},
		{ m_pos + glm::vec3( 0.5f, -0.5f,  0.5f), {1.0f, 0.0f, bottom}},
		{ m_pos + glm::vec3(-0.5f, -0.5f, -0.5f), {0.0f, 1.0f, bottom}},
		{ m_pos + glm::vec3( 0.5f, -0.5f, -0.5f), {1.0f, 1.0f, bottom}},

		// left
		{ m_pos + glm::vec3(-0.5f, -0.5f, -0.5f), {0.0f, 0.0f, side}},
		{ m_pos + glm::vec3(-0.5f, -0.5f,  0.5f), {1.0f, 0.0f, side}},
		{ m_pos + glm::vec3(-0.5f,  0.5f, -0.5f), {0.0f, 1.0f, side}},
		{ m_pos + glm::vec3(-0.5f,  0.5f,  0.5f), {1.0f, 1.0f, side}},

		// right									   
		{ m_pos + glm::vec3(0.5f, -0.5f, -0.5f), {0.0f, 0.0f, side}},
		{ m_pos + glm::vec3(0.5f, -0.5f,  0.5f), {1.0f, 0.0f, side}},
		{ m_pos + glm::vec3(0.5f,  0.5f, -0.5f), {0.0f, 1.0f, side}},
		{ m_pos + glm::vec3(0.5f,  0.5f,  0.5f), {1.0f, 1.0f, side}}
	};
}
