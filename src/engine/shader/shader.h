#pragma once

#include <glm/glm.hpp>

namespace Engine
{
	class Shader
	{
	public:
		Shader(const char* vPath, const char* fPath);
		
		void setMat4vf(const char* uName, glm::mat4 matrix);
		void setFloat(const char* uName, float value);
		void setInt(const char* uName, int value);
		void setVec3vf(const char* uName, const glm::vec3 vector);

		void use() const;

		Shader() = default;
		Shader(Shader&&) = default;
		Shader& operator=(Shader&&) = default;
		~Shader();

		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;

	private:
		unsigned int ID;
	};
}