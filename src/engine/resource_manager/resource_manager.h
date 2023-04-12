#pragma once

#include <map>
#include <vector>
#include <string>

namespace Engine
{
	class Shader;
	class Texture;
	class TextureCube;
	class TextureArray;

	class ResourceManager
	{
	public:
		void free();

		static ResourceManager& getInstance();

		Shader& getShader(const char* sName);
		void setShader(const char* sName, const char* vPath, const char* fPath);
		Texture& getTexture(const char* tName);
		void setTexture(const char* tName, const char* tPath);
		TextureCube& getTextureCube(const char* tName);
		void setTextureCube(const char* tName, std::vector<std::string> facePaths);		
		TextureArray& getTextureArray();
		void setTextureArray(std::vector<std::string> tPaths);


		ResourceManager(const ResourceManager&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;
		ResourceManager(ResourceManager&&) = delete;
		ResourceManager& operator=(ResourceManager&&) = delete;
	private:
		static ResourceManager* s_manager;

		static std::map<const char*, Shader> s_shaders;
		static std::map<const char*, Texture> s_textures;
		static std::map<const char*, TextureCube> s_textureCubes;
		static TextureArray s_texArray;

		ResourceManager() = default;
		~ResourceManager() = default;
	};
}

