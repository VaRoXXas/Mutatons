#include "pch.h"
#include "GameObject/GameObject.h"
#include "Component.h"
#include "Collisions.h"
#include "Rendering/animation/Animation.h"
#include "Rendering/animation/Animator.h"
#include "Components/ColliderComponent.h"
#include "Components/GraphicsComponent.h"
#include "Components/TransformComponent.h"
#include "GameObject/Crossing.h"

// posPtr = new glm::vec3();
// sizePtr = new glm::vec3();


Crossing::~Crossing()
{
}
//refers to the game object which crossing is colliding with, and changes its move direction based on private "dir" propety
void Crossing::ChangeDirection(GameObject *gameObject)
{
	if (this->IsActive())
	{
		glm::vec3* posPtr = gameObject->GetColliderComponent()->GetPosRef();
		glm::vec3* sizePtr = gameObject->GetColliderComponent()->GetSizeRef();
		if (this->GetColliderComponent()->Collides(*posPtr, *sizePtr))
		{
			gameObject->SetDirection(dir);
		}
	}
}

//This function interprets input from mouse
void Crossing::InputDirection()
{
	if (GetClick() && !once && !availableDirs.empty())
	{
		once = true;
		if (dir != availableDirs[0])
			SetDir(availableDirs[0]);
		else if (dir == availableDirs[0])
			SetDir(availableDirs[1]);
	}
	else
		once = false;
		
}

//Dir setter
void Crossing::SetDir(std::string str)
{
	dir = str;
}

//adds directions to the list
void Crossing::AddDir(std::string str)
{
	availableDirs.push_back(str);
}