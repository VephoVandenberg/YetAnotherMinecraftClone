#include "../../engine/renderer/renderer.h"

#include "block.h"

using namespace Engine;
using namespace Data;
using namespace GameModules;

constexpr unsigned int g_numberOfFaces = 36;

Block::Block(glm::vec3 pos, Texture& texture)
	: m_pos(pos)
	, m_texture(texture)
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
}

/*
void Block::render(Renderer& renderer, Shader& shader, glm::mat4 view)
{
	renderer.render(m_pos, shader, m_texture, view);
}
*/
