#pragma once

#include <map>
#include <vector>
#include <string>

namespace Engine
{
	class Shader;
	class Texture;
	class TextureCube;

	class ResourceManager
	{
	public:
		
		static ResourceManager& getInstance();

		Shader& getShader(const char* sName);
		void setShader(const char* sName, const char* vPath, const char* fPath);
		Texture& getTexture(const char* tName);
		void setTexture(const char* sName, const char* tPath);
		TextureCube& getTextureCube(const char* tName);
		void setTextureCube(const char* sName, std::vector<std::string> facePaths);


		ResourceManager(const ResourceManager&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;
		ResourceManager(ResourceManager&&) = delete;
		ResourceManager& operator=(ResourceManager&&) = delete;
	private:
		static ResourceManager* s_manager;

		static std::map<const char*, Shader> s_shaders;
		static std::map<const char*, Texture> s_textures;
		static std::map<const char*, TextureCube> s_textureCubes;

		ResourceManager() = default;
		~ResourceManager() = default;
	};
}

