// ReSharper disable CppUseAuto
#include "pch.h"
#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
: position(position), front(glm::vec3(0.0f, 0.0f, -1.0f)), worldUp(up), yaw(yaw), pitch(pitch), movementSpeed(SPEED_DEFAULT), mouseSensitivity(SENSITIVITY_DEFAULT), zoom(ZOOM_DEFAULT)
{
    UpdateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
: position(glm::vec3(posX, posY, posZ)), front(glm::vec3(0.0f, 0.0f, -1.0f)), worldUp(glm::vec3(upX, upY, upZ)), yaw(yaw), pitch(pitch), movementSpeed(SPEED_DEFAULT), mouseSensitivity(SENSITIVITY_DEFAULT), zoom(ZOOM_DEFAULT)
{
    UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(position, position + front, up);
}

void Camera::ProcessKeyboard(const CameraDirection direction, const float deltaTime)
{
    const float velocity = movementSpeed * deltaTime;

    switch(direction)
    {
	    default:
	        break;
    }
	
    if (direction == Forward)
        position += front * velocity;
    if (direction == Backward)
        position -= front * velocity;
    if (direction == Left)
        position -= right * velocity;
    if (direction == Right)
        position += right * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    // update front, right and up Vectors using the updated Euler angles
    UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
    zoom -= (float)yoffset;
    if (zoom < 1.0f)
        zoom = 1.0f;
    if (zoom > 90.0f)
        zoom = 90.0f;
}

void Camera::UpdateCameraVectors()
{
    // calculate the new front vector
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);
	
    // also re-calculate the right and up vector
    right = glm::normalize(glm::cross(front, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement
    up = glm::normalize(glm::cross(right, front));
}
