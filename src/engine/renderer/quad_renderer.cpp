#include <glad/glad.h>

#include "../shader/shader.h"
#include "../texture/texture.h"

#include "quad_renderer.h"

using namespace Engine;

QuadRenderer::QuadRenderer()
{
	init();
}

void QuadRenderer::init()
{
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f,  1.0f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 2,
		1, 2, 3
	};

	glCreateVertexArrays(1, &m_buffer.VAO);
	glCreateBuffers(1, &m_buffer.VBO);
	glCreateBuffers(1, &m_buffer.IBO);

	glBindVertexArray(m_buffer.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer.VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer.IBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void QuadRenderer::render(Shader& shader, Texture& texture)
{
	shader.use();
	texture.bind();
	glBindVertexArray(m_buffer.VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
