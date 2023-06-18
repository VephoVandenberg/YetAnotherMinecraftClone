#include <cstddef>
#include <glm/gtc/matrix_transform.hpp>

#include "mesh.h"

using namespace Engine;

Mesh::Mesh(std::vector<Vertex> vertices)
	: m_vertices(vertices)
{
	init();
}

void Mesh::init()
{
	glCreateVertexArrays(1, &m_buffer.VAO);
	glCreateBuffers(1, &m_buffer.VBO);

	glBindVertexArray(m_buffer.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer.VBO);

	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex),
		nullptr, GL_DYNAMIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices.size() * sizeof(Vertex), m_vertices.data());

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 
		sizeof(Vertex), (void*)(0));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 
		sizeof(Vertex), (void*)(offsetof(Vertex, textureCoords)));
	glEnableVertexAttribArray(1);
	
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
		sizeof(Vertex), (void*)(offsetof(Vertex, normal)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE,
		sizeof(Vertex), (void*)(offsetof(Vertex, brightness)));
	glEnableVertexAttribArray(3);

}

void Mesh::updateData(std::vector<Vertex> verticies)
{
	m_vertices = verticies;

	glBindVertexArray(m_buffer.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer.VBO);

	glBufferSubData(GL_ARRAY_BUFFER, 0, 
		m_vertices.size() * sizeof(Vertex), m_vertices.data());
}
