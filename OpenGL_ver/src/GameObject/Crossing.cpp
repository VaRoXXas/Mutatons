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

extern GameObject* gameObjectPtr;
extern std::vector<Model*> vecModel;
extern std::vector<GameObject*> gameObjectVector;
glm::vec3 tempLoc, tempLoc2;


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
			if ((dir == "right" && gameObject->GetDirection() == "forward") || (dir =="left" && gameObject->GetDirection() == "forward"))
			{
				posPtr->z = posPtr->z - 0.001;
				gameObject->GetTransformComponent()->SetLocation(*posPtr);
			}
			else if ((dir == "right" && gameObject->GetDirection() == "back") || (dir == "left" && gameObject->GetDirection() == "back"))
			{
				posPtr->z = posPtr->z + 0.001;
				gameObject->GetTransformComponent()->SetLocation(*posPtr);
			}
			else if ((dir == "forward" && gameObject->GetDirection() == "right") || (dir == "back" && gameObject->GetDirection() == "right"))
			{
				posPtr->x = posPtr->x + 0.001;
				gameObject->GetTransformComponent()->SetLocation(*posPtr);
			}
			else if ((dir == "forward" && gameObject->GetDirection() == "left") || (dir == "back" && gameObject->GetDirection() == "left"))
			{
				posPtr->x = posPtr->x - 0.001;
				gameObject->GetTransformComponent()->SetLocation(*posPtr);
			}
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

//Dir setter, this function also sets
void Crossing::SetDir(std::string str)
{
	dir = str;

	if (!bridges.empty())
	{
		if (bridges[0]->GetTag() == dir)
		{
			tempLoc = bridges[0]->GetTransformComponent()->GetLocation();
			tempLoc.y = -0.5f;
			bridges[0]->GetTransformComponent()->SetLocation(tempLoc);
			//bridges[0]->GetTransformComponent()->SetRotation(-90.0f,'z');
			tempLoc = bridges[1]->GetTransformComponent()->GetLocation();
			tempLoc.y = 0.0f;
			bridges[1]->GetTransformComponent()->SetLocation(tempLoc);
		}
		else if (bridges[1]->GetTag() == dir)
		{
			tempLoc = bridges[1]->GetTransformComponent()->GetLocation();
			tempLoc.y = -0.5f;
			bridges[1]->GetTransformComponent()->SetLocation(tempLoc);
			tempLoc = bridges[0]->GetTransformComponent()->GetLocation();
			tempLoc.y = 0.0f;
			bridges[0]->GetTransformComponent()->SetLocation(tempLoc);
		}
	}
}

//adds directions to the list
void Crossing::AddDir(std::string str)
{
	tempLoc = glm::vec3(0.0f);
	availableDirs.push_back(str);
	if (str == "right")
	{
		tempLoc.x = -1.f;
		gameObjectPtr = new GameObject;
		gameObjectPtr->SetActive();
		gameObjectPtr->SetTag("right");
		gameObjectPtr->AddComponent(std::make_shared<TransformComponent>());
		gameObjectPtr->AddComponent(std::make_shared<GraphicsComponent>());
		gameObjectPtr->GetTransformComponent()->SetLocation(tempLoc);
		gameObjectPtr->GetGraphicsComponent()->SetModel(vecModel[13]);
		bridges.push_back(gameObjectPtr);
		this->AddChild(gameObjectPtr);
	}
	if (str == "left")
	{
		tempLoc.x = 1.f;
		gameObjectPtr = new GameObject;
		gameObjectPtr->SetActive();
		gameObjectPtr->SetTag("left");
		gameObjectPtr->AddComponent(std::make_shared<TransformComponent>());
		gameObjectPtr->AddComponent(std::make_shared<GraphicsComponent>());
		gameObjectPtr->GetTransformComponent()->SetLocation(tempLoc);
		gameObjectPtr->GetGraphicsComponent()->SetModel(vecModel[13]);
		bridges.push_back(gameObjectPtr);
		this->AddChild(gameObjectPtr);
	}
	if (str == "forward")
	{
		tempLoc.x = 1.f;
		gameObjectPtr = new GameObject;
		gameObjectPtr->SetActive();
		gameObjectPtr->SetTag("forward");
		gameObjectPtr->AddComponent(std::make_shared<TransformComponent>(tempLoc));
		gameObjectPtr->AddComponent(std::make_shared<GraphicsComponent>());
		gameObjectPtr->GetGraphicsComponent()->SetModel(vecModel[13]);
		bridges.push_back(gameObjectPtr);
		this->AddChild(gameObjectPtr);
	}
	if (str == "back")
	{
		tempLoc.x = -1.f;
		gameObjectPtr = new GameObject;
		gameObjectPtr->SetActive();
		gameObjectPtr->SetTag("back");
		gameObjectPtr->AddComponent(std::make_shared<TransformComponent>(tempLoc));
		gameObjectPtr->AddComponent(std::make_shared<GraphicsComponent>());
		gameObjectPtr->GetGraphicsComponent()->SetModel(vecModel[13]);
		bridges.push_back(gameObjectPtr);
		this->AddChild(gameObjectPtr);
	}
}
