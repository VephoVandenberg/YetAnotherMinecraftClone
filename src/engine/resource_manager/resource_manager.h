#pragma once

#include <map>

namespace Engine
{
	class Shader;

	class ResourceManager
	{
	public:
		
		static ResourceManager& getInstance();

		static Shader& getShader(const char* sName);
		static void setShader(const char* sName, const char* vFile, const char* fFile);

		ResourceManager(const ResourceManager&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;
		ResourceManager(ResourceManager&&) = delete;
		ResourceManager& operator=(ResourceManager&&) = delete;
	private:
		static ResourceManager* s_manager;

		static std::map<const char*, Shader> s_shaders;

		ResourceManager() = default;
		~ResourceManager() = default;
	};
}

