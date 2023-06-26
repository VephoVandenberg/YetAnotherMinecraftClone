#pragma once

#include <map>
#include <vector>
#include <string>

namespace Engine
{
	namespace TextureNames
	{
		const char* g_grass = "grass";
		const char* g_stone = "stone";
		const char* g_sand = "sand";
		const char* g_snow = "snow";
		const char* g_wood = "wood";
		const char* g_leaf = "leaf";
	}

	const std::map<const char*, const char*> g_texturePaths= {
		
	};

	static const std::vector<std::string> s_texturePaths = {
		"textures/cubes/grass_dirt/dirt_grass.png",		// 0 side
		"textures/cubes/grass_dirt/dirt.png",			// 1 side, bottom, top
		"textures/cubes/grass_dirt/grass.png",			// 2 top

		"textures/cubes/stone/stone.png",				// 3 side, bottom, top

		"textures/cubes/sand/sand.png",					// 4 side, bottom, top
		
		"textures/cubes/snow/snow.png",					// 5 side, bottom, top

		"textures/cubes/wood/wood_side_2.png",			// 6 side
		"textures/cubes/wood/wood_top_bottom.png",		// 7 top, bottom

		"textures/cubes/wood/leaf_0.png"				// 8 side, bottom, top
	};
}
