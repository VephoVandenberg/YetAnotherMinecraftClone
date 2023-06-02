#pragma once

#include <map>

namespace Engine
{
	namespace ShaderNames
	{
		inline const char* g_base_shader = "base_shader";
		inline const char* g_cube_shader = "cube_shader";
	}

	using ShaderPaths = std::map<const char*, std::pair<const char*, const char*>>;
	const ShaderPaths g_shaderPaths = {
		{ShaderNames::g_base_shader, {"shaders/base_shader.vert", "shaders/base_shader.frag"}},
		{ShaderNames::g_cube_shader, {"shaders/cube_shader.vert", "shaders/cube_shader.frag"}}
	};
}