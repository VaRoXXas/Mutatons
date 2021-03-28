#pragma once

namespace Input
{
	void ProcessInput(GLFWwindow* window);
	void MouseCallback(GLFWwindow* window, double xpos, double ypos);
	void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
}