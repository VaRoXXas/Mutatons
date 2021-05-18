#pragma once

#include <camera.h>



class MousePicker 
{
private:

	glm::vec3 currentRay;
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	Camera* cameraPtr;
	GLFWwindow* windowPtr;
	glm::vec3 currentTerrainPoint;

	glm::vec3 CalculateMouseRay();

	glm::vec3 ToWorldCoords(glm::vec4 eyeCoords);

	glm::vec3 GetNormalizedDevideCoords(float mouseX, float mouseY);

	glm::vec3 GetPointOnRay(glm::vec3 ray, float distance);

	glm::vec3 BinarySearch(int count, float start, float finish, glm::vec3 ray);

	bool IntersectionInRange(float start, float finish, glm::vec3 ray);

	bool IsUnderGround(glm::vec3 testPoint);

public:
	MousePicker(Camera* camPtr, GLFWwindow* wPtr);

	void SetProjectionMatrix(glm::mat4 projection);

	glm::vec3 GetCurrentRay();

	glm::vec3 GetCurrentTerrainPoint();

	void Update();
};