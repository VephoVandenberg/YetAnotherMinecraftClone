#include "image_loader/stb_image.h"

#include <iostream>
#include <glad/glad.h>

#include "texture_cube.h"

using namespace Engine;

TextureCube::TextureCube(std::vector<std::string>& faces)
{
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &m_width, &m_height, &m_nrChannels, 0);
		if (data)
		{
			if (faces[i].find("png") != -1)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
					m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA,
					m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			}
		}
		else
		{
			std::cout << "ERROR::CUBEMAP_PATH_FAILED_TO_LOAD:" << faces[i] << std::endl;
			exit(EXIT_FAILURE);
		}
		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void TextureCube::bind()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
}
