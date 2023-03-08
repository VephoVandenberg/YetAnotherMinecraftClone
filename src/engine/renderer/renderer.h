#pragma once

namespace Engine
{
	class Renderer
	{
	public:
		Renderer();

		~Renderer() = default;

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) = delete;
	};
}

