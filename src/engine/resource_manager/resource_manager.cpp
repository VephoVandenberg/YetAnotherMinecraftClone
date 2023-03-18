#include <iostream>

#include "../shader/shader.h"

#include "resource_manager.h"

using namespace Engine;

ResourceManager* ResourceManager::s_manager = nullptr;
std::map<const char*, Shader> ResourceManager::s_shaders;

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
	if (s_shaders.find(sName) != s_shaders.end())
	{
		std::cout << "Could not find shader with key: " << sName << std::endl;
		exit(EXIT_FAILURE);
	}

	return s_shaders[sName];
}
