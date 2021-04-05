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
    float x = posX;
    UpdateCameraVectors();
}

const glm::mat4& Camera::GetViewMatrix() const
{
    return glm::lookAt(position, position + front, up);
}

void Camera::ProcessKeyboard(const CameraDirection& direction, const float& deltaTime)
{
    const float velocity = movementSpeed * deltaTime;
    
    switch(direction)
    {
		case CameraDirection::Forward:
            position += front * velocity;
            break;
        case CameraDirection::Backward:
            position -= front * velocity;
            break;
        case CameraDirection::Left:
            position -= right * velocity;
            break;
        case CameraDirection::Right:
            position += right * velocity;
            break;
	    default:
	        break;
    }
}

void Camera::ProcessMouseMovement(float& xoffset, float& yoffset, const GLboolean constrainPitch)
{
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped.
    if (constrainPitch)
    {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    // Update front, right and up vectors using the updated Euler angles.
    UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(const float& yoffset)
{
    zoom -= static_cast<float>(yoffset);
	
    if (zoom < 1.0f)
        zoom = 1.0f;
    if (zoom > 90.0f)
        zoom = 90.0f;
}

void Camera::UpdateCameraVectors()
{
    // Calculate the new front vector.
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);
	
    // Also re-calculate the right and up vector.
    right = glm::normalize(glm::cross(front, worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    up = glm::normalize(glm::cross(right, front));
}
