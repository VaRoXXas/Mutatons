#include "pch.h"
#include "Components/TransformComponent.h"


TransformComponent::TransformComponent(glm::vec3 targetLocation)
{
/* set up location, rotation and scale */
	transform = glm::mat4(1.0f);
	transform = glm::translate(transform, targetLocation);
}

TransformComponent::TransformComponent()
{
	transform = glm::mat4(1.0f);
}

void TransformComponent::SetLocation(glm::vec3 targetLocation)
{
	transform = glm::mat4(1.0f);
	transform = glm::translate(transform, targetLocation);
}