#include "pch.h"
#include "GameObject/GameObject.h"
#include "Component.h"
#include "Collisions.h"
#include "Components/ColliderComponent.h"
#include "Components/GraphicsComponent.h"
#include "Components/TransformComponent.h"
#include "GameObject/Crossing.h"



//refers to the game object which crossing is colliding with, and changes its move direction based on private "dir" propety
void Crossing::ChangeDirection(GameObject *gameObject)
{
	glm::vec3* posPtr = &gameObject->GetColliderComponent()->GetPos();
	glm::vec3* sizePtr = &gameObject->GetColliderComponent()->GetSize();
	if (this->GetColliderComponent()->Collides(*posPtr, *sizePtr))
	{
		gameObject->SetDirection(dir);
	}
}

//This function will interpret input from mouse
void Crossing::InputDirection()
{
	//Input 
}

//Dir setter
void Crossing::SetDir(std::string str)
{
	dir = str;
}