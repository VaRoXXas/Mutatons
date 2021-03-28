#pragma once

namespace Util
{
	// Windows
	
	void HideTerminal();
	void ShowTerminal();

	// GLFW
	
	void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
	void EnableWireframeMode();
	void DisableWireframeMode();
}