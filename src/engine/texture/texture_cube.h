#pragma once

#include <vector>
#include <string>

namespace Engine
{
	class TextureCube
	{
	public:
		TextureCube(std::vector<std::string>& faces);

		void bind();
		
		TextureCube() = default;
		~TextureCube() = default;
		TextureCube(TextureCube&&) = default;
		TextureCube& operator=(TextureCube&&) = default;

		TextureCube(const TextureCube&) = delete;
		TextureCube& operator=(const TextureCube&) = delete;
	
	private:
		unsigned int ID;

		int m_width;
		int m_height;
		int m_nrChannels;
	};
}
