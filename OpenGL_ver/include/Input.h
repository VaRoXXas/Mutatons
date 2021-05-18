#pragma once

namespace Input
{
	// Process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly.
	void ProcessInput(GLFWwindow* windowPtr);
	void CursorPosCallback(GLFWwindow* windowPtr, const double xpos, const double ypos);
	void ScrollCallback(GLFWwindow* windowPtr, const double xoffset, const double yoffset);
	void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	// input actions
	void ToggleCursor();
	void ToggleSceneExplorationMode();
	void MoveCameraForward();
	void MoveCameraBackward();
	void MoveCameraLeft();
	void MoveCameraRight();
}
