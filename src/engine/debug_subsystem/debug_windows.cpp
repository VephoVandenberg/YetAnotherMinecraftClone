#include "debug_windows.h"

using namespace Engine;

DebugWindows* DebugWindows::m_instance = nullptr;

DebugWindows::DebugWindows()
	: m_io(&ImGui::GetIO())
{}

DebugWindows& DebugWindows::getInstance()
{
	if (!m_instance)
	{
		m_instance = new DebugWindows();
	}
	return *m_instance;
}

void DebugWindows::setFramerate()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void DebugWindows::getFramerate() const
{
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowSize(ImVec2(400.0f, 50.0f));
	ImGui::Begin("Debug");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_io->Framerate, m_io->Framerate);
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
