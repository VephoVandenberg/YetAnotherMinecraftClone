#pragma once

#include <vector>
#include <string>

namespace Engine
{
	class TextureArray
	{
	public:
		TextureArray(const std::vector<std::string>& tPaths, int width, int height);

		~TextureArray();

		void bind();

		TextureArray() = default;
		TextureArray(TextureArray&&) = default;
		TextureArray& operator=(TextureArray&&) = default;

		TextureArray(const TextureArray&) = delete;
		TextureArray& operator=(const TextureArray&) = delete;

	private:
		unsigned int ID;

		int m_width, m_height, m_nrChannels;
	};
}

