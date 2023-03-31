#include "../../engine/resource_manager/resource_manager.h"

#include "block.h"

using namespace Engine;
using namespace Data;
using namespace GameModules;

constexpr unsigned int g_numberOfFaces = 36;

Block::Block(glm::vec3 pos, BlockType type)
	: m_pos(pos)
	, m_type(type)
{
	// front
	m_vertices.push_back({ m_pos + glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f) });
	m_vertices.push_back({ m_pos + glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 0.0f) });
	m_vertices.push_back({ m_pos + glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(0.0f, 1.0f) });
	m_vertices.push_back({ m_pos + glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 1.0f) });

	// back
	m_vertices.push_back({ m_pos + glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f) });
	m_vertices.push_back({ m_pos + glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f) });
	m_vertices.push_back({ m_pos + glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f) });
	m_vertices.push_back({ m_pos + glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f) });

	// top
	m_vertices.push_back({ m_pos + glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(0.0f, 0.0f) });
	m_vertices.push_back({ m_pos + glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f) });
	m_vertices.push_back({ m_pos + glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f) });
	m_vertices.push_back({ m_pos + glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f) });

	// bottom
	m_vertices.push_back({ m_pos + glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f) });
	m_vertices.push_back({ m_pos + glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 0.0f) });
	m_vertices.push_back({ m_pos + glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f) });
	m_vertices.push_back({ m_pos + glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 1.0f) });

	// left
	m_vertices.push_back({ m_pos + glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f) });
	m_vertices.push_back({ m_pos + glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 0.0f) });
	m_vertices.push_back({ m_pos + glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f) });
	m_vertices.push_back({ m_pos + glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 1.0f) });

	// right
	m_vertices.push_back({ m_pos + glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f) });
	m_vertices.push_back({ m_pos + glm::vec3(0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 0.0f) });
	m_vertices.push_back({ m_pos + glm::vec3(0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f) });
	m_vertices.push_back({ m_pos + glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 1.0f) });


	switch (m_type)
	{
	case BlockType::Grass:
	case BlockType::Dirt:
	case BlockType::Stone:
	{
		m_texture = std::move(ResourceManager::getInstance().getTexture("grass"));
	}break;

	case BlockType::Air:
		break;
	}
}
