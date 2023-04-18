#include <cstddef>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "../shader/shader.h"
#include "../texture/texture.h"
#include "../texture/texture_cube.h"
#include "../texture/texture_array.h"

#include "mesh.h"

using namespace Engine;

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indicies)
	: m_vertices(vertices)
	, m_indicies(indicies)
{
	init();
}

void Mesh::init()
{
	glCreateVertexArrays(1, &m_buffer.VAO);
	glCreateBuffers(1, &m_buffer.VBO);
	glCreateBuffers(1, &m_buffer.IBO);

	glBindVertexArray(m_buffer.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer.VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer.IBO);

	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), 
		m_vertices.data(), GL_DYNAMIC_DRAW);
	// 6 becaus each face has 2 triangles, thus 6 vertices
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * m_vertices.size() * sizeof(unsigned int), 
		nullptr, GL_DYNAMIC_DRAW);

	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_indicies.size() * sizeof(unsigned int), 
		m_indicies.data());

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 
		sizeof(Vertex), (void*)(offsetof(Vertex, textureCoords)));
	glEnableVertexAttribArray(1);
}

void Mesh::draw(Shader& shader, Texture& texture, glm::mat4 cameraView)
{
	glBindVertexArray(m_buffer.VAO);
	shader.use();
	shader.setMat4vf("u_view", cameraView);
	texture.bind();
	glBindVertexArray(m_buffer.VAO);
	glDrawElements(GL_TRIANGLES, m_indicies.size(), GL_UNSIGNED_INT, nullptr);
}

void Mesh::draw(Shader& shader, TextureCube& textureCube, glm::mat4 cameraView)
{
	glBindVertexArray(m_buffer.VAO);
	shader.use();
	shader.setMat4vf("u_view", cameraView);
	textureCube.bind();
	glBindVertexArray(m_buffer.VAO);
	glDrawElements(GL_TRIANGLES, m_indicies.size(), GL_UNSIGNED_INT, nullptr);
}

void Mesh::draw(Shader& shader, TextureArray& textureArray, glm::mat4 cameraView)
{
	glBindVertexArray(m_buffer.VAO);
	shader.use();
	shader.setMat4vf("u_view", cameraView);
	textureArray.bind();
	glBindVertexArray(m_buffer.VAO);
	glDrawElements(GL_TRIANGLES, m_indicies.size(), GL_UNSIGNED_INT, nullptr);
}

void Mesh::updateData(std::vector<Vertex> verticies, std::vector<unsigned int> indicies)
{
	m_vertices = verticies;
	m_indicies = indicies;

	glBindVertexArray(m_buffer.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer.VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer.IBO);

	glBufferSubData(GL_ARRAY_BUFFER, 0, 
		m_vertices.size() * sizeof(Vertex), m_vertices.data());
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 
		m_indicies.size() * sizeof(unsigned int), m_indicies.data());
}

void Mesh::appendData(std::vector<Vertex> verticies, std::vector<unsigned int> indicies)
{
	m_indicies.insert(m_indicies.end(), indicies.begin(), indicies.end());

	glBindVertexArray(m_buffer.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer.IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer.IBO);

	glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices.size(), m_vertices.data());
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_indicies.size(), m_indicies.data());
}
