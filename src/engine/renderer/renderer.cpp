#include <glm/gtc/matrix_transform.hpp>

#include <glad/glad.h>

#include "../shader/shader.h"
#include "../texture/texture.h"

#include "renderer.h"

using namespace Engine;

using namespace Engine;

Renderer::Renderer()
{
	init();
}

void Renderer::init()
{
	float vertices[] = {
		 // front
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // 0
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // 1
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // 2
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // 3

		 // back
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // 4
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // 5
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // 6
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // 7

		 // top
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, // 8
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // 9
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // 10
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // 11

		 // bottom
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // 12
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // 13
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // 14
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // 15

		 // left
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // 16
		-0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // 17
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // 18
		-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // 19

		 // right
		 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // 20
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // 21
		 0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // 22
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // 23

	};

	unsigned int indicies[] = {
		// front
		0, 1, 2,
		1, 3, 2,

		// back
		4, 6, 5,
		5, 6, 7,

		// top
		8, 9, 10,
		9, 11, 10,
		
		// bottom
		12, 14, 13,
		13, 14, 15,

		// left
		16, 17, 18,
		17, 19, 18,

		// right
		20, 22, 21,
		21, 22, 23,
	};

	glCreateVertexArrays(1, &m_buffer.VAO);
	glCreateBuffers(1, &m_buffer.VBO);
	glCreateBuffers(1, &m_buffer.IBO);

	glBindVertexArray(m_buffer.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer.VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer.IBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void Renderer::render(glm::vec3 pos, Shader& shader, Texture& texture, glm::mat4 view)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, pos);

	shader.use();
	glCullFace(GL_CULL_FACE);
	shader.setMat4vf("u_view", view);
	shader.setMat4vf("u_model", model);
	texture.bind();
	glBindVertexArray(m_buffer.VAO);
	glDrawElements(GL_TRIANGLES, 72, GL_UNSIGNED_INT, nullptr);
}
