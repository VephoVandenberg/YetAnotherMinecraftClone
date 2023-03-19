#pragma once

namespace Engine
{
	class Texture
	{
	public:
		Texture(const char* tPath);

		~Texture();

		void bind();

		Texture() = default;
		Texture(Texture&&) = default;
		Texture& operator=(Texture&&) = default;

		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;

	private:
		unsigned int ID;

		int m_width, m_height;
		int m_nrChannels;
	};
}

