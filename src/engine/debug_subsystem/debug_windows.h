#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace Engine
{
	class DebugWindows
	{
	public:
		static DebugWindows& getInstance();

		void setFramerate();
		void getFramerate() const;

	private:
		DebugWindows();
		~DebugWindows() = default;

		static DebugWindows* m_instance;

		const ImGuiIO* m_io;
	};
}
