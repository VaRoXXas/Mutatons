#pragma once

namespace Util
{
	// Windows
	
	void HideTerminal();
	void ShowTerminal();

	// GLFW

	// Whenever the window size changes (by OS or user resize) this callback function executes.
	void FramebufferSizeCallback(GLFWwindow* window, const int width, const int height);

	// OpenGL
	
	// Wireframe is an "x-ray" mode, where you see individual vertices and edges through non-transparent faces.
	void EnableWireframeMode();
	void DisableWireframeMode();
}
