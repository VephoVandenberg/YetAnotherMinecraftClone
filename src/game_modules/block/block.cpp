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
	m_vertices = {
		// front
		{ m_pos + glm::vec3(-0.5f, -0.5f,  0.5f), {-0.5f, -0.5f,  0.5f} },
		{ m_pos + glm::vec3(0.5f, -0.5f,  0.5f), { 0.5f, -0.5f,  0.5f} },
		{ m_pos + glm::vec3(-0.5f,  0.5f,  0.5f), {-0.5f,  0.5f,  0.5f} },
		{ m_pos + glm::vec3(0.5f,  0.5f,  0.5f), { 0.5f,  0.5f,  0.5f} },
		
		// back
		{ m_pos + glm::vec3(-0.5f, -0.5f, -0.5f), { -0.5f, -0.5f, -0.5f} },
		{ m_pos + glm::vec3(0.5f, -0.5f, -0.5f), {  0.5f, -0.5f, -0.5f} },
		{ m_pos + glm::vec3(-0.5f,  0.5f, -0.5f), { -0.5f,  0.5f, -0.5f} },
		{ m_pos + glm::vec3(0.5f,  0.5f, -0.5f), {  0.5f,  0.5f, -0.5f} },
		
		// top
		{ m_pos + glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-0.5f,  0.5f,  0.5f) },
		{ m_pos + glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3( 0.5f,  0.5f,  0.5f) },
		{ m_pos + glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(-0.5f,  0.5f, -0.5f) },
		{ m_pos + glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3( 0.5f,  0.5f, -0.5f) },
																		
		// bottom
		{ m_pos + glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(-0.5f, -0.5f,  0.5f) },
		{ m_pos + glm::vec3(0.5f,  -0.5f,  0.5f), glm::vec3( 0.5f, -0.5f,  0.5f) },
		{ m_pos + glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, -0.5f) },
		{ m_pos + glm::vec3(0.5f,  -0.5f, -0.5f), glm::vec3( 0.5f, -0.5f, -0.5f) },
		
		// left
		{ m_pos + glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, -0.5f) },
		{ m_pos + glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(-0.5f, -0.5f,  0.5f) },
		{ m_pos + glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(-0.5f,  0.5f, -0.5f) },
		{ m_pos + glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-0.5f,  0.5f,  0.5f) },
		
		// right
		{ m_pos + glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f)  },
		{ m_pos + glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.5f, -0.5f,  0.5f)  },
		{ m_pos + glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.5f,  0.5f, -0.5f)  },
		{ m_pos + glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.5f,  0.5f,  0.5f)  }
	};

	switch (m_type)
	{
	case BlockType::Grass:
	case BlockType::Dirt:
	case BlockType::Stone:
	{
		m_texture = std::move(ResourceManager::getInstance().getTextureCube("dirt"));
	}break;

	case BlockType::Air:
		break;
	}
}
