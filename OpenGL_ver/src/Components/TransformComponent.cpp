#include "pch.h"
#include "Component.h"
#include "Components/TransformComponent.h"



TransformComponent::TransformComponent(glm::vec3 targetLocation)
{
	location = targetLocation;
	transform = glm::mat4(1.0f);
	transform = glm::translate(transform, targetLocation);
}

TransformComponent::TransformComponent()
{
	transform = glm::mat4(1.0f);
}

glm::mat4 TransformComponent::GetTransform()
{
	return transform;
}
glm::vec3 TransformComponent::GetLocation()
{
	return location;
}

glm::vec3 TransformComponent::GetScale()
{
	return scale;
}

void TransformComponent::SetLocation(glm::vec3 &targetLocation)
{
	location = targetLocation;
	transform = glm::mat4(1.0f);
	transform = glm::translate(transform, targetLocation);
}

void TransformComponent::SetRotation(float degrees, char rotationDirection)
{
	//transform = glm::mat4(1.0f);
	glm::vec3 direction;
	if (rotationDirection == 'x')
		direction = glm::vec3(1.0, 0.0, 0.0);
	else if (rotationDirection == 'y')
		direction = glm::vec3(0.0, 1.0, 0.0);
	else if (rotationDirection == 'z')
		direction = glm::vec3(0.0, 0.0, 1.0);
	transform = glm::rotate(transform, glm::radians(degrees), direction);
}

void TransformComponent::SetScale(float &targetScale)
{
	scale = glm::vec3(targetScale, targetScale, targetScale);
	transform = glm::scale(transform, scale);
}

void TransformComponent::SetScale(glm::vec3 &targetScale)
{
	scale = targetScale;
	//transform = glm::mat4(1.0f);
	transform = glm::scale(transform, scale);
}

std::string TransformComponent::GetType()
{
	return type;
}

void TransformComponent::DisableComponent() {
	enabled = false;
}

void TransformComponent::EnableComponent() {
	enabled = true;
}

bool TransformComponent::Enabled() {
	return enabled;
}
