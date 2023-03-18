#pragma once

namespace Engine
{
	class Shader
	{
	public:
		Shader(const char* vPath, const char* fPath);

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