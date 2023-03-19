#pragma once

#include <map>

namespace Engine
{
	namespace TextureNames
	{
		const char* g_grass = "grass";
	}

	const std::map<const char*, const char*> g_texturePaths= {
		{TextureNames::g_grass, "textures/grass.png"}
	};

}
