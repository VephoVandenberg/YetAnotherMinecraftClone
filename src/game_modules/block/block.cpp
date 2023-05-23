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
	, front(false)
	, back(false)
	, top(false)
	, bottom(false)
	, right(false)
	, left(false)
{
	switch (m_type)
	{
	case BlockType::GrassDirt:
	{
		topT_ind = 2.0f;
		bottomT_ind = 1.0f;
		sideT_ind = 0.0f;
	}break;

	case BlockType::Dirt:
	{
		topT_ind = bottomT_ind = sideT_ind = 1.0f;
	}break;

	default:
	{}break;
	}
}
