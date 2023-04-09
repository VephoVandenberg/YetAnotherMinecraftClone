#include <iostream>

#include "../shader/shader.h"
#include "../texture/texture.h"
#include "../texture/texture_cube.h"

#include "resource_manager.h"

using namespace Engine;

ResourceManager* ResourceManager::s_manager = nullptr;
std::map<const char*, Shader> ResourceManager::s_shaders;
std::map<const char*, Texture> ResourceManager::s_textures;
std::map<const char*, TextureCube> ResourceManager::s_textureCubes;

ResourceManager& ResourceManager::getInstance()
{
	if (!s_manager)
	{
		s_manager = new ResourceManager();
	}

	return *s_manager;
}

void ResourceManager::setShader(const char* sName, const char* vName, const char* fName)
{
	s_shaders[sName] = Shader(vName, fName);
}

Shader& ResourceManager::getShader(const char* sName)
{
	if (s_shaders.find(sName) == s_shaders.end())
	{
		std::cout << "ERROR::KEY_NOT_FOUND::SHADERS: " << sName << std::endl;
		exit(EXIT_FAILURE);
	}

	return s_shaders[sName];
}

Texture& ResourceManager::getTexture(const char* tName)
{
	if (s_textures.find(tName) == s_textures.end())
	{
		std::cout << "ERROR::KEY_NOT_FOUND::TEXTURES: " << tName << std::endl;
		exit(EXIT_FAILURE);
	}

	return s_textures[tName];
}

void ResourceManager::setTexture(const char* sName, const char* tPath)
{
	s_textures[sName] = Texture(tPath);
}

TextureCube& ResourceManager::getTextureCube(const char* tName)
{
	if (s_textureCubes.find(tName) == s_textureCubes.end())
	{
		std::cout << "ERROR::KEY_NOT_FOUND::TEXTURE_CUBES: " << tName << std::endl;
		exit(EXIT_FAILURE);
	}

	return s_textureCubes[tName];
}

void ResourceManager::setTextureCube(const char* tName, std::vector<std::string> facePaths)
{
	s_textureCubes[tName] = TextureCube(facePaths);
}
