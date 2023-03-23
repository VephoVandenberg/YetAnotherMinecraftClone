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

		Shader& getShader(const char* sName);
		void setShader(const char* sName, const char* vPath, const char* fPath);
		Texture& getTexture(const char* tName);
		void setTexture(const char* sName, const char* tPath);

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

