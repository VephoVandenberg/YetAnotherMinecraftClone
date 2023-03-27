#pragma once

#include <glm/glm.hpp>

namespace Engine
{
	class Shader;
	class Texture;

	struct Buffer
	{
		float vertices[16];
		unsigned int indicies[36];
		unsigned int VAO;
		unsigned int VBO;
		unsigned int IBO;
	};

	class Renderer
	{
	public:
		Renderer();
		~Renderer() = default;

		void render(glm::vec3 pos, Shader& shader, Texture& texture, glm::mat4 view);

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) = delete;

	private:
		void init();

		Buffer m_buffer;
	};
}

