#include <iostream>
#include <glad/glad.h>

#include "image_loader/stb_image.h"

#include "texture_array.h"

using namespace Engine;

TextureArray::TextureArray(const std::vector<std::string>& tPaths, int iWidth, int iHeight)
{
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D_ARRAY, ID);
	
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, iWidth, iHeight, tPaths.size());

	for (unsigned int i = 0; i < tPaths.size(); i++)
	{
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(tPaths[i].c_str(), &m_width, &m_height, &m_nrChannels, 0);
		
		if (!data)
		{
			std::cout << "Could not upload the data" << std::endl;
			exit(EXIT_FAILURE);
		}
		
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i,
			iWidth, iHeight, 1, GL_RGB, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

TextureArray::~TextureArray()
{

}

void TextureArray::bind()
{
	glBindTexture(GL_TEXTURE_2D_ARRAY, ID);
}
