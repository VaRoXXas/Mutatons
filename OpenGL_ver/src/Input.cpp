#include "pch.h"
#include "Input.h"
#include "Settings.h"
#include "Camera.h"

extern Camera mainCamera;
extern GLfloat deltaTime;
extern void (*rKeyAction)();
extern void (*tKeyAction)();
extern void (*eKeyAction)();
extern void (*qKeyAction)();
extern void (*wKeyAction)();
extern void (*sKeyAction)();
extern void (*aKeyAction)();
extern void (*dKeyAction)();



void Input::ProcessInput(GLFWwindow* windowPtr)
{
	// We use if-cascade instead of switch, because we want to be able to press multiple keys in a single frame.
	
	if (glfwGetKey(windowPtr, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(windowPtr, true);

	if (glfwGetKey(windowPtr, GLFW_KEY_W) == GLFW_PRESS)
		wKeyAction();
	if (glfwGetKey(windowPtr, GLFW_KEY_S) == GLFW_PRESS)
		sKeyAction();
	if (glfwGetKey(windowPtr, GLFW_KEY_A) == GLFW_PRESS)
		aKeyAction();
	if (glfwGetKey(windowPtr, GLFW_KEY_D) == GLFW_PRESS)
		dKeyAction();

	// special actions
	if (glfwGetKey(windowPtr, GLFW_KEY_R) == GLFW_PRESS)
		rKeyAction();
	if (glfwGetKey(windowPtr, GLFW_KEY_T) == GLFW_PRESS)
		tKeyAction();
	if (glfwGetKey(windowPtr, GLFW_KEY_E) == GLFW_PRESS)
		eKeyAction();
	if (glfwGetKey(windowPtr, GLFW_KEY_Q) == GLFW_PRESS)
		qKeyAction();
}

void Input::CursorPosCallback(GLFWwindow* windowPtr, const double xpos, const double ypos)
{
	static float lastX = WINDOW_WIDTH / 2, lastY = WINDOW_HEIGHT / 2;
	static auto firstMouse = true;

	if (firstMouse)
	{
		lastX = static_cast<float>(xpos);
		lastY = static_cast<float>(ypos);
		firstMouse = false;
	}

	auto xoffset = static_cast<float>(xpos) - lastX;
	auto yoffset = lastY - static_cast<float>(ypos); // Reversed since y-coordinates go from bottom to top.

	lastX = static_cast<float>(xpos);
	lastY = static_cast<float>(ypos);

	mainCamera.ProcessMouseMovement(xoffset, yoffset);
}

void Input::ScrollCallback(GLFWwindow* windowPtr, const double xoffset, const double yoffset)
{
	mainCamera.ProcessMouseScroll(static_cast<float>(yoffset));
}
