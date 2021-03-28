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

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void Util::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
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
