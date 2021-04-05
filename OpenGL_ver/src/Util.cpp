#include "pch.h"
#include "Util.h"

void Util::HideTerminal()
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);
}

void Util::ShowTerminal()
{
	ShowWindow(GetConsoleWindow(), SW_SHOWNORMAL);
}

void Util::FramebufferSizeCallback(GLFWwindow* window, const int width, const int height)
{
    // Make sure the viewport matches the new window dimensions.
    // Note that width and height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void Util::EnableWireframeMode()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Util::DisableWireframeMode()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
