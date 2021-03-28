#include "pch.h"
#include "Input.h"
#include "Settings.h"
#include "Camera.h"

extern Camera camera;
extern GLfloat deltaTime;
extern void (*RKeyAction)();
extern void (*TKeyAction)();
extern void (*EKeyAction)();
extern void (*QKeyAction)();
extern void (*WKeyAction)();
extern void (*SKeyAction)();
extern void (*AKeyAction)();
extern void (*DKeyAction)();

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void Input::ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		WKeyAction();
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		SKeyAction();
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		AKeyAction();
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		DKeyAction();

	// special actions
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		RKeyAction();
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		TKeyAction();
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		EKeyAction();
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		QKeyAction();
}

void Input::MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	static GLfloat lastX = WINDOW_WIDTH / 2, lastY = WINDOW_HEIGHT / 2;
	static bool firstMouse = true;

	if (firstMouse)
	{
		lastX = (GLfloat)xpos;
		lastY = (GLfloat)ypos;
		firstMouse = false;
	}

	float xoffset = (GLfloat)xpos - lastX;
	float yoffset = lastY - (GLfloat)ypos; // reversed since y-coordinates go from bottom to top

	lastX = (GLfloat)xpos;
	lastY = (GLfloat)ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void Input::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll((GLfloat)yoffset);
}
