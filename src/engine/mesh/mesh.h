#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "data.h"

namespace Engine
{
	struct Buffer
	{
		unsigned int VAO;
		unsigned int VBO;
		unsigned int IBO;
	};


	class Shader;
	class Texture;
	class TextureCube;

	using namespace Data;

	class Mesh
	{
	public:
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indicies);

		void draw(Shader& shader, Texture& texture, glm::mat4 cameraView);
		void draw(Shader& shader, TextureCube& texture, glm::mat4 cameraView);
		void updateData(std::vector<Vertex> vertices, std::vector<unsigned int> indicies);
		void appendData(std::vector<Vertex> vertices, std::vector<unsigned int> indicies);

		Mesh() = default;
		~Mesh() = default;
		Mesh(Mesh&&) = default;
		Mesh& operator=(Mesh&&) = default;

		Mesh(const Mesh&) = delete;
		Mesh& operator=(const Mesh&) = delete;

	private:
		void init();

		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_indicies;

		Buffer m_buffer;
	};
}
