#pragma once

#include <map>
#include <vector>
#include <string>

namespace Engine
{
	namespace TextureNames
	{
		const char* g_grass = "grass";
	}

	const std::map<const char*, const char*> g_texturePaths= {
		
	};

	static const std::vector<std::string> s_texturePaths = {
		"textures/cubes/grass_dirt/dirt_grass.png",
		"textures/cubes/grass_dirt/dirt.png",
		"textures/cubes/grass_dirt/grass.png",

		"textures/cubes/stone/stone.png"
	};
}
