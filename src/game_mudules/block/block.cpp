#include "../../engine/renderer/renderer.h"

#include "block.h"

using namespace Engine;
using namespace GameModules;

Block::Block(glm::vec3 pos, Texture& texture)
	: m_pos(pos)
	, m_texture(texture)
{}

void Block::render(Renderer& renderer, Shader& shader, glm::mat4 view)
{
	renderer.render(m_pos, shader, m_texture, view);
}
