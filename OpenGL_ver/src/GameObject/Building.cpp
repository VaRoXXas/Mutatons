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
#include <AudioManager.h>

extern GameObject* gameObjectPtr;
extern std::vector<Model*> vecModel;
glm::vec3 temp = glm::vec3(0.0f);

extern sf::SoundBuffer mutationSoundBuffer, controlPanelSoundBuffer, dyingFromLaserSoundBuffer;
extern AudioManager audioManager;

//this function is used to call, suitable to the Building type, actions
void Building::Reaction(GameObject* gameObject)
{
	if (this->IsActive() && this->GetColliderComponent()->Enabled())
	{
		glm::vec3* posPtr = gameObject->GetColliderComponent()->GetPosRef();
		glm::vec3* sizePtr = gameObject->GetColliderComponent()->GetSizeRef();
		if (this->GetColliderComponent()->Collides(*posPtr, *sizePtr))
		{
			if (type == "Laboratory")
			{
				if (gameObject->GetElement() != this->GetElement())
				{
					audioManager.PlaySfSound(mutationSoundBuffer);
				}
				gameObject->SetElement(this->GetElement());
			}
			else if (type == "Obstacle")
			{
				if (gameObject->GetElement() == this->GetElement())
				{
					if (gameObject->IsActive() == true)
					{
						audioManager.PlaySfSound(dyingFromLaserSoundBuffer);
					}
					gameObject->Destroy();
					gameObject->GetColliderComponent()->DisableComponent();
					this->GetColliderComponent()->DisableComponent();
					//this->SetInactive();
					for (GameObject*  cl : childrenLasers)
					{
						cl->GetGraphicsComponent()->SetModel(vecModel[34]);
					}
				}
				else if (gameObject->GetElement() != this->GetElement())
				{
					if (gameObject->IsActive() == true)
					{
						audioManager.PlaySfSound(dyingFromLaserSoundBuffer);
					}
					gameObject->Destroy();
					gameObject->GetColliderComponent()->DisableComponent();
				}
			}
			else if (type == "Control")
			{
				if ((!childrenObstacles.empty() && CheckObstacles()) || childrenObstacles.empty())
				{
					if (captured == false)
					{
						audioManager.PlaySfSound(controlPanelSoundBuffer);
					}
					captured = true;
					this->GetGraphicsComponent()->SetModel(vecModel[45]);
					//gameObject->SetVelocity(0.0f);
					gameObject->Destroy();
					gameObject->GetColliderComponent()->DisableComponent();
					gameObject->SetIsInControl(true);
				}
			}
		}
	}
}

//setters and getters
void Building::SetType(std::string typeSet)
{
	glm::vec3 tempLoc = this->GetTransformComponent()->GetLocation();
	type = typeSet;
	if (type == "Obstacle")
	{
		tempLoc.z = tempLoc.z + 0.35f;
		tempLoc.x = tempLoc.x + 0.35f;
		this->GetColliderComponent()->Initialize(tempLoc, glm::vec3(0.3f));
		if (this->GetElement() == 1)
		{
			laserModel = 31;
		}
		else if (this->GetElement() == 2)
		{
			laserModel = 32;
		}
		else if (this->GetElement() == 3)
		{
			laserModel = 33;
		}
	}
	if (type == "Laboratory")
	{
		tempLoc.z = tempLoc.z + 0.9f;
		tempLoc.x = tempLoc.x + 0.9f;
		this->GetColliderComponent()->Initialize(tempLoc,glm::vec3(0.1f));
		if (this->GetElement() == 1)
		{
			this->GetGraphicsComponent()->SetModel(vecModel[42]);
		}
		else if (this->GetElement() == 2)
		{
			this->GetGraphicsComponent()->SetModel(vecModel[43]);
		}
		else if (this->GetElement() == 3)
		{
			this->GetGraphicsComponent()->SetModel(vecModel[44]);
		}
	}

	if (type == "Control")
	{
		glm::vec3 tempLoc = this->GetTransformComponent()->GetLocation();
		tempLoc.z = tempLoc.z + 0.4f;
		tempLoc.x = tempLoc.x + 0.4f;
		this->GetColliderComponent()->Initialize(tempLoc, glm::vec3(0.1f));
		this->GetGraphicsComponent()->SetModel(vecModel[46]);
	}
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
		if (!b->GetColliderComponent()->Enabled())
			inactiveObstacles = inactiveObstacles + 1;
	}

	if (inactiveObstacles == childrenObstacles.size())
		return true;
	else return false;
}

void Building::AddLaserSide(std::string side)
{
	if (side == "back")
	{
		gameObjectPtr = new GameObject;
		gameObjectPtr->SetActive();
		gameObjectPtr->SetTag("back");
		gameObjectPtr->AddComponent(std::make_shared<TransformComponent>());
		gameObjectPtr->AddComponent(std::make_shared<GraphicsComponent>());
		gameObjectPtr->GetTransformComponent()->SetLocation(temp);
		gameObjectPtr->GetGraphicsComponent()->SetModel(vecModel[laserModel]);
		childrenLasers.push_back(gameObjectPtr);
		this->AddChild(gameObjectPtr);
	} 
	else if (side == "front")
	{
		gameObjectPtr = new GameObject;
		gameObjectPtr->SetActive();
		gameObjectPtr->SetTag("front");
		gameObjectPtr->AddComponent(std::make_shared<TransformComponent>());
		gameObjectPtr->AddComponent(std::make_shared<GraphicsComponent>());
		gameObjectPtr->GetTransformComponent()->SetLocation(temp);
		gameObjectPtr->GetTransformComponent()->SetRotation(180.f, 'y');
		gameObjectPtr->GetGraphicsComponent()->SetModel(vecModel[laserModel]);
		childrenLasers.push_back(gameObjectPtr);
		this->AddChild(gameObjectPtr);
	}
	else if (side == "right")
	{
		gameObjectPtr = new GameObject;
		gameObjectPtr->SetActive();
		gameObjectPtr->SetTag("right");
		gameObjectPtr->AddComponent(std::make_shared<TransformComponent>());
		gameObjectPtr->AddComponent(std::make_shared<GraphicsComponent>());
		gameObjectPtr->GetTransformComponent()->SetLocation(temp);
		gameObjectPtr->GetTransformComponent()->SetRotation(90.f, 'y');
		gameObjectPtr->GetGraphicsComponent()->SetModel(vecModel[laserModel]);
		childrenLasers.push_back(gameObjectPtr);
		this->AddChild(gameObjectPtr);
	}
	else if (side == "left")
	{
		gameObjectPtr = new GameObject;
		gameObjectPtr->SetActive();
		gameObjectPtr->SetTag("left");
		gameObjectPtr->AddComponent(std::make_shared<TransformComponent>());
		gameObjectPtr->AddComponent(std::make_shared<GraphicsComponent>());
		gameObjectPtr->GetTransformComponent()->SetLocation(temp);
		gameObjectPtr->GetTransformComponent()->SetRotation(-90.f, 'y');
		gameObjectPtr->GetGraphicsComponent()->SetModel(vecModel[laserModel]);
		childrenLasers.push_back(gameObjectPtr);
		this->AddChild(gameObjectPtr);
	}
}

bool Building::GetCaptured()
{
	return captured;
}