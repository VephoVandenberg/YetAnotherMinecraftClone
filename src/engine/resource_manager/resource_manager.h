#pragma once

#include <map>

namespace Engine
{
	class Shader;
	class Texture;

	class ResourceManager
	{
	public:
		
		static ResourceManager& getInstance();

		static Shader& getShader(const char* sName);
		static void setShader(const char* sName, const char* vPath, const char* fPath);
		static Texture& getTexture(const char* tName);
		static void setTexture(const char* sName, const char* tPath);

		ResourceManager(const ResourceManager&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;
		ResourceManager(ResourceManager&&) = delete;
		ResourceManager& operator=(ResourceManager&&) = delete;
	private:
		static ResourceManager* s_manager;

		static std::map<const char*, Shader> s_shaders;
		static std::map<const char*, Texture> s_textures;

		ResourceManager() = default;
		~ResourceManager() = default;
	};
}

