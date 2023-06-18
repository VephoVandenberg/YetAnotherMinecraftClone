#pragma once

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "../shader/shader.h"
#include "../texture/texture.h"
#include "../texture/texture_cube.h"
#include "../texture/texture_array.h"

#include "data.h"

namespace Engine
{
	struct Buffer
	{
		unsigned int VAO;
		unsigned int VBO;
	};


	class Shader;
	class Texture;
	class TextureCube;
	class TextureArray;

	using namespace Data;

	class Mesh
	{
	public:
		Mesh(std::vector<Vertex> vertices);

		template <typename T>
		void draw(Shader& shader, T& texture, glm::mat4 cmaeraView);

		void updateData(std::vector<Vertex> vertices);

		Mesh() = default;
		~Mesh() = default;
		Mesh(Mesh&&) = default;
		Mesh& operator=(Mesh&&) = default;

		Mesh(const Mesh&) = delete;
		Mesh& operator=(const Mesh&) = delete;

	private:
		void init();

		std::vector<Vertex> m_vertices;
		Buffer m_buffer;
	};

	template <typename T>
	void Mesh::draw(Shader& shader, T& textureObj, glm::mat4 cmaeraView)
	{
		glBindVertexArray(m_buffer.VAO);
		shader.use();
		shader.setMat4vf("u_view", cmaeraView);
		textureObj.bind();
		glBindVertexArray(m_buffer.VAO);
		glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
	}
}
