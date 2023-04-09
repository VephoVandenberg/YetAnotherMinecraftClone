#pragma once

#include <map>
#include <vector>
#include <string>

namespace Engine
{
	namespace TextureCubeNames
	{
		const char* g_dirt = "dirt";
	}

	using facePaths = std::vector<std::string>;
	const std::map<const char*, facePaths> g_textureCubePaths = {
		{
			TextureCubeNames::g_dirt,	{
				"textures/cubes/grass_dirt/dirt_grass.png", // +x
				"textures/cubes/grass_dirt/dirt_grass.png", // -x
				"textures/cubes/grass_dirt/grass.png",		// +y
				"textures/cubes/grass_dirt/dirt.png",		// -y
				"textures/cubes/grass_dirt/dirt_grass.png",	// +z
				"textures/cubes/grass_dirt/dirt_grass.png"	// -z
			}
		}
	};

}
