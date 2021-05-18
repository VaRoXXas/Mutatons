#include "pch.h"
#include "MousePicker.h"
#include "Settings.h"

int RECURSION_COUNT = 200;
float RAY_RANGE = 600;



glm::vec3 MousePicker::CalculateMouseRay()
{
	double xpos, ypos;
	glfwGetCursorPos(windowPtr, &xpos, &ypos);
	float mouseX = xpos;
	float mouseY = ypos;

	glm::vec3 normalizedCoords = GetNormalizedDevideCoords(mouseX, mouseY);
	glm::vec4 clipCoords = glm::vec4(normalizedCoords.x, normalizedCoords.y, -1.f, 1.0f);
	glm::vec4 eyeCoords = glm::inverse(projectionMatrix) * clipCoords;
	eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0, 0.0);
	glm::vec3 worldRay = ToWorldCoords(eyeCoords);
	return worldRay;
}

glm::vec3 MousePicker::ToWorldCoords(glm::vec4 eyeCoords)
{
	glm::vec3 rayWorld = glm::vec3((glm::inverse(viewMatrix) * eyeCoords).x, (glm::inverse(viewMatrix) * eyeCoords).y, (glm::inverse(viewMatrix) * eyeCoords).z);
	// normalise the vector
	rayWorld = glm::normalize(rayWorld);

	return rayWorld;
}

glm::vec3 MousePicker::GetNormalizedDevideCoords(float mouseX, float mouseY)
{
	float x = (2.f * mouseX) / WINDOW_WIDTH - 1;
	float y = 1.0f - (2.0f * mouseY) / WINDOW_HEIGHT;
	float z = 1.0f;
	glm::vec3 rayNDS = glm::vec3(x, y, z);

	return rayNDS;
}

glm::vec3 MousePicker::GetPointOnRay(glm::vec3 ray, float distance)
{
	glm::vec3 camPos = cameraPtr->GetPosition();

	glm::vec3 start(camPos.x, camPos.y, camPos.z);
	glm::vec3 scaledRay(ray.x * distance, ray.y * distance, ray.z * distance);


	return glm::vec3(start.x + scaledRay.x, start.y + scaledRay.y, start.z + scaledRay.z);
}

glm::vec3 MousePicker::BinarySearch(int count, float start, float finish, glm::vec3 ray)
{
	float half = start + ((finish - start) / 2.f);
	if (count >= RECURSION_COUNT) {
		glm::vec3 endPoint = GetPointOnRay(ray, half);
		//Terrain terrain = getTerrain(endPoint.getX(), endPoint.getZ());
		glm::vec2 terrain = glm::vec2(endPoint.x, endPoint.z);
		if (terrain != glm::vec2(NULL)) {
			return endPoint;
		}
		else {
			return glm::vec3(NULL);
		}
	}
	if (IntersectionInRange(start, half, ray)) {
		return BinarySearch(count + 1, start, half, ray);
	}
	else {
		return BinarySearch(count + 1, half, finish, ray);
	}
}

bool MousePicker::IntersectionInRange(float start, float finish, glm::vec3 ray)
{
	glm::vec3 startPoint = GetPointOnRay(ray, start);
	glm::vec3 endPoint = GetPointOnRay(ray, finish);

	if (!IsUnderGround(startPoint) && IsUnderGround(endPoint)) {
		return true;
	}
	else {
		return false;
	}
}

bool MousePicker::IsUnderGround(glm::vec3 testPoint)
{
	//Terrain terrain = getTerrain(testPoint.getX(), testPoint.getZ());
	glm::vec2 terrain = glm::vec2(testPoint.x, testPoint.z);
	float height = 0.0f;
	//if (terrain != null) {
	//	height = terrain.getHeightOfTerrain(testPoint.getX(), testPoint.getZ());
	//}
	if (testPoint.y < height) {
		return true;
	}
	else {
		return false;
	}
}

MousePicker::MousePicker(Camera* camPtr, GLFWwindow* wPtr)
{
	this->cameraPtr = camPtr;

	cameraPtr->GetViewMatrix();
	this->windowPtr = wPtr;
}

void MousePicker::SetProjectionMatrix(glm::mat4 projection)
{
	this->projectionMatrix = projection;
}

glm::vec3 MousePicker::GetCurrentRay()
{
	return currentRay;
}

glm::vec3 MousePicker::GetCurrentTerrainPoint()
{
	return currentTerrainPoint;
}

void MousePicker::Update()
{
	viewMatrix = cameraPtr->GetViewMatrix();
	currentRay = CalculateMouseRay();

	if (IntersectionInRange(0, RAY_RANGE, currentRay)) {
		currentTerrainPoint = BinarySearch(0, 0, RAY_RANGE, currentRay);
	}
	else {
		currentTerrainPoint = glm::vec3(NULL);
	}
}
