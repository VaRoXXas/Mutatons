#include "pch.h"
#include "Component.h"
#include "Collisions.h"
#include "Components/TransformComponent.h"
#include "Components/ColliderComponent.h"




std::string ColliderComponent::GetType()
{
	return ColliderComponent::type;
}
void ColliderComponent::DisableComponent()
{
	enabled = false;
}
void ColliderComponent::EnableComponent()
{
	enabled = true;
}
bool ColliderComponent::Enabled()
{
	return enabled;
}

glm::vec3 ColliderComponent::GetPos()
{
	return pos;
}

glm::vec3 ColliderComponent::GetSize()
{
	return size;
}

void ColliderComponent::SetTransformPtr(std::shared_ptr<TransformComponent> componentPtr)
{
	transformComponentPtr = componentPtr;
}
void ColliderComponent::CollisionCalc()
{
	//transform = &transformComponentPtr->GetTransform();
	*transform = transformComponentPtr->GetTransform();
	pos = col.PosCalc(*transform);
	size = col.SizeCalc(*transform);
}