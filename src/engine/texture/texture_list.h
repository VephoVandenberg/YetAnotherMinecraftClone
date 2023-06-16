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
	}

	const std::map<const char*, const char*> g_texturePaths= {
		
	};

	static const std::vector<std::string> s_texturePaths = {
		"textures/cubes/grass_dirt/dirt_grass.png",
		"textures/cubes/grass_dirt/dirt.png",
		"textures/cubes/grass_dirt/grass.png",

		"textures/cubes/stone/stone.png",

		"textures/cubes/sand/sand.png",
		
		"textures/cubes/snow/snow.png"
	};
}
