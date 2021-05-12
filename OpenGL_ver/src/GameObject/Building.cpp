#include "pch.h"
#include "GameObject/GameObject.h"
#include "Component.h"
#include "Collisions.h"
#include "Rendering/animation/Animation.h"
#include "Rendering/animation/Animator.h"
#include "Components/ColliderComponent.h"
#include "Components/GraphicsComponent.h"
#include "Components/TransformComponent.h"
#include "GameObject/Building.h"



//this function is used to call, suitable to the Building type, actions
void Building::Reaction(GameObject* gameObject)
{
	if (this->IsActive())
	{
		glm::vec3* posPtr = gameObject->GetColliderComponent()->GetPosRef();
		glm::vec3* sizePtr = gameObject->GetColliderComponent()->GetSizeRef();
		if (this->GetColliderComponent()->Collides(*posPtr, *sizePtr))
		{
			if (type == "Laboratory")
			{
				gameObject->SetElement(element);
			}
			else if (type == "Obstacle")
			{
				if (gameObject->GetElement() == element)
				{
					gameObject->SetInactive();
					this->SetInactive();
				}
				else if (gameObject->GetElement() != element)
				{
					gameObject->SetInactive();
				}
			}
			else if (type == "Control")
			{
				if (!childrenObstacles.empty() && CheckObstacles())
					captured = true;
				else if(childrenObstacles.empty())
					captured = true;
			}
		}
	}
}

//setters and getters
void Building::SetType(std::string typeSet)
{
	type = typeSet;
}

void Building::SetBuildingElement(int elem)
{
	element = elem;
}

std::string Building::GetType()
{
	return type;
}

//this function adds chosen obstacles to the control point
void Building::AddChildObstacle(Building* building)
{
	childrenObstacles.push_back(building);
}

//this function checks if all the assigned to the control point obstacles are inactive or active and returns results
bool Building::CheckObstacles()
{
	int inactiveObstacles = 0;
	for (Building* b : childrenObstacles)
	{
		if (!b->IsActive())
			inactiveObstacles = inactiveObstacles + 1;
	}

	if (inactiveObstacles == childrenObstacles.size())
		return true;
	else return false;
}