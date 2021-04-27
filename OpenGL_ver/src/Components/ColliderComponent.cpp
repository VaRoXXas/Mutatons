#include "pch.h"
#include "Component.h"
#include "Collisions.h"
#include "Components/TransformComponent.h"
#include "Components/ColliderComponent.h"



//returns component type
std::string ColliderComponent::GetType()
{
	return ColliderComponent::type;
}

//for disabling and enabling component
void ColliderComponent::DisableComponent()
{
	enabled = false;
}
void ColliderComponent::EnableComponent()
{
	enabled = true;
}

//returns if component is enabled
bool ColliderComponent::Enabled()
{
	return enabled;
}

//returns collider position
glm::vec3 ColliderComponent::GetPos()
{
	return pos;
}

//returns colllider size
glm::vec3 ColliderComponent::GetSize()
{
	return size;
}

//function used to set pointer to GameObject's transform component and initialize Collider's propeties
void ColliderComponent::Initialize(std::shared_ptr<TransformComponent> componentPtr)
{
	transformComponentPtr = componentPtr;
	pos = transformComponentPtr->GetLocation();
	size = transformComponentPtr->GetScale();
}

//This function is used to check if two gameobjects collide
void ColliderComponent::Collides(glm::vec3 &collidingObjectLocation, glm::vec3 &collidingObjectSize)
{
	if (col.Collides(pos, size, collidingObjectLocation, collidingObjectSize))
	{
		std::cout << "kolizja" << std::endl;
	}
}