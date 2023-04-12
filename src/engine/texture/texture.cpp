#include <string>
#include <iostream>
#include <glad/glad.h>

#include "image_loader/stb_image.h"	
#include "texture.h"

using namespace Engine;

Texture::Texture(const char* tPath)
{
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(tPath, &m_width, &m_height, &m_nrChannels, 0);

	if (data)
	{
		if (std::string(tPath).find("png") != -1)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ERROR::TEXTURE::FAILED_TO_LOAD" << std::endl;
		exit(EXIT_FAILURE);
	}

	stbi_image_free(data);

}

Texture::~Texture()
{

}

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, ID);
	glActiveTexture(GL_TEXTURE0);
}
