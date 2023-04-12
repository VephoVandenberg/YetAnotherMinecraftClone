#pragma once

#include <map>
#include <vector>
#include <string>

namespace Engine
{
	namespace TextureCubeNames
	{
		const char* g_dirt_grass = "dirt_grass";
		const char* g_dirt = "dirt";
	}

	using facePaths = std::vector<std::string>;
	const std::map<const char*, facePaths> g_textureCubePaths = {
	};
}
