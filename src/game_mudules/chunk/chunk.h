#pragma once

namespace GameModules
{
	class Chunk
	{
	public:

		Chunk() = default;
		~Chunk() = default;

		Chunk(const Chunk&) = delete;
		Chunk& operator=(const Chunk&) = delete;
		Chunk(Chunk&&) = delete;
		Chunk& operator=(Chunk&&) = delete;

	private:
	};
}