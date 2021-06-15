// ReSharper disable CppUseAuto
// ReSharper disable CppRedundantQualifier
#include "pch.h"
#include "Camera.h"
#include "FrustumCulling/Frustum.h"

Frustum frustum;

Camera::Camera(bool isometric, float yaw, float pitch)
: isometric(isometric), position(glm::vec3(12.0f, 10.0f, 0.0f)), front(glm::vec3(-1.0f, -3.0f, -1.0f)), worldUp(glm::vec3(0.0f, 1.0f, 0.0f)), yaw(yaw), pitch(pitch), movementSpeed(S_SPEED_DEFAULT), mouseSensitivity(S_SENSITIVITY_DEFAULT), zoom(S_ZOOM_DEFAULT)
{
    if (isometric == false)
    {
        UpdateCameraVectors();
    }
    if (isometric == true)
    {
        up = glm::vec3(0.0f, 1.0f, 0.0f);
    }

    
}

/*Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
: position(glm::vec3(posX, posY, posZ)), front(glm::vec3(0.0f, 0.0f, -1.0f)), worldUp(glm::vec3(upX, upY, upZ)), yaw(yaw), pitch(pitch), movementSpeed(S_SPEED_DEFAULT), mouseSensitivity(S_SENSITIVITY_DEFAULT), zoom(S_ZOOM_DEFAULT)
{
    UpdateCameraVectors();
}*/

glm::mat4 Camera::GetViewMatrix() const
{
    //frustum culling
    Vec3 p, f, u, pf;
    p.x = position.x;
    p.y = position.y;
    p.z = position.z;
    f.x = front.x;
    f.y = front.y;
    f.z = front.z;
    u.x = up.x;
    u.y = up.y;
    u.z = up.z;
    pf = p + f;
    frustum.SetCamDef(p, pf, u);

    return glm::lookAt(position, position + front, up);
}

void Camera::ProcessKeyboard(const CameraDirection& direction, const float& deltaTime)
{
    const float velocity = movementSpeed * deltaTime;
    
    if (isometric == 0)
    {
        switch (direction)
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
    if (isometric == 1)
    {
        switch (direction)
        {
        case CameraDirection::Forward:
            if(position.x >= 0 && position.z >= -10)
                position -= glm::vec3(1.0f, 0.0f, 1.0f) * velocity;
            break;
        case CameraDirection::Backward:
            if (position.x <= 30 && position.z <= 15)
                position += glm::vec3(1.0f, 0.0f, 1.0f) * velocity;
            break;
        case CameraDirection::Left:
            if (position.x >= 0 && position.z <= 15)
                position -= glm::vec3(1.0f, 0.0f, -1.0f) * velocity;
            break;
        case CameraDirection::Right:
            if (position.x <= 30 && position.z >= -10)
                position += glm::vec3(1.0f, 0.0f, -1.0f) * velocity;
            break;
        default:
            break;
        }
    }
}

void Camera::ProcessMouseMovement(float& xoffset, float& yoffset, const bool constrainPitch)
{
    if (isometric == 0)
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
    
}

void Camera::ProcessMouseScroll(const float& yoffset)
{
    zoom -= static_cast<float>(yoffset);
	
    if (zoom < 1.0f)
        zoom = 1.0f;
    if (zoom > 90.0f)
        zoom = 90.0f;
}

const glm::vec3& Camera::GetPosition() const
{
	return position;
}

void Camera::SetPosition(const glm::vec3 position)
{
	this->position = position;
}

const glm::vec3& Camera::GetFront() const
{
	return front;
}

void Camera::SetFront(const glm::vec3 front)
{
	this->front = front;
}

const glm::vec3& Camera::GetUp() const
{
	return up;
}

void Camera::SetUp(const glm::vec3 up)
{
	this->up = up;
}

const glm::vec3& Camera::GetRight() const
{
	return right;
}

void Camera::SetRight(const glm::vec3 right)
{
	this->right = right;
}

const glm::vec3& Camera::GetWorldUp() const
{
	return worldUp;
}

void Camera::SetWorldUp(const glm::vec3 worldUp)
{
	this->worldUp = worldUp;
}

float Camera::GetYaw() const
{
	return yaw;
}

void Camera::SetYaw(const float yaw)
{
	this->yaw = yaw;
}

float Camera::GetPitch() const
{
	return pitch;
}

void Camera::SetPitch(const float pitch)
{
	this->pitch = pitch;
}

float Camera::GetMovementSpeed() const
{
	return movementSpeed;
}

void Camera::SetMovementSpeed(const float movementSpeed)
{
	this->movementSpeed = movementSpeed;
}

float Camera::GetMouseSensitivity() const
{
	return mouseSensitivity;
}

void Camera::SetMouseSensitivity(const float mouseSensitivity)
{
	this->mouseSensitivity = mouseSensitivity;
}

float Camera::GetZoom() const
{
	return zoom;
}

void Camera::SetZoom(const float zoom)
{
	this->zoom = zoom;
}

bool Camera::GetIsometric()
{
    return isometric;
}

void Camera::SetIsometric(bool isometric)
{
    this->isometric = isometric;
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
