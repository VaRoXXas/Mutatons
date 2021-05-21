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
glm::vec3 tempLoc;
std::vector<GameObject*> bridges;

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

//Dir setter
void Crossing::SetDir(std::string str)
{
	dir = str;
	for (GameObject *br : bridges)
	{
		if (br->GetTag() == str)
		{
			tempLoc = br->GetTransformComponent()->GetLocation();
			tempLoc.y = 0.5f;
			br->GetTransformComponent()->SetLocation(tempLoc);
		}
		else
		{
			tempLoc = br->GetTransformComponent()->GetLocation();
			tempLoc.y = 1.0f;
			br->GetTransformComponent()->SetLocation(tempLoc);
		}
	}
}

//adds directions to the list
void Crossing::AddDir(std::string str)
{
	availableDirs.push_back(str);
	if (str == "right")
	{
		tempLoc = this->GetInputLocation();
		tempLoc.x = tempLoc.x - 1.f;
		gameObjectPtr = new GameObject;
		gameObjectPtr->SetActive();
		gameObjectPtr->SetTag("right");
		//gameObjectPtr->SetVelocity(15.0f);
		gameObjectPtr->AddComponent(std::make_shared<TransformComponent>());
		gameObjectPtr->AddComponent(std::make_shared<GraphicsComponent>());
		gameObjectPtr->GetTransformComponent()->SetLocation(tempLoc);
		gameObjectPtr->GetGraphicsComponent()->SetModel(vecModel[13]);
		bridges.push_back(gameObjectPtr);
		gameObjectVector[0]->AddChild(gameObjectPtr);
	}
	if (str == "left")
	{
		tempLoc = this->GetInputLocation();
		tempLoc.x = tempLoc.x + 1.f;
		gameObjectPtr = new GameObject;
		gameObjectPtr->SetActive();
		gameObjectPtr->SetTag("left");
		//gameObjectPtr->SetVelocity(15.0f);
		gameObjectPtr->AddComponent(std::make_shared<TransformComponent>());
		gameObjectPtr->AddComponent(std::make_shared<GraphicsComponent>());
		gameObjectPtr->GetTransformComponent()->SetLocation(tempLoc);
		gameObjectPtr->GetGraphicsComponent()->SetModel(vecModel[13]);
		bridges.push_back(gameObjectPtr);
		gameObjectVector[0]->AddChild(gameObjectPtr);
	}
	if (str == "forward")
	{
		tempLoc = this->GetInputLocation();
		tempLoc.z = tempLoc.z + 1.f;
		gameObjectPtr = new GameObject;
		gameObjectPtr->SetActive();
		gameObjectPtr->SetTag("forward");
		//gameObjectPtr->SetVelocity(15.0f);
		gameObjectPtr->AddComponent(std::make_shared<TransformComponent>(tempLoc));
		gameObjectPtr->AddComponent(std::make_shared<GraphicsComponent>());
		//gameObjectPtr->GetTransformComponent()->SetLocation();
		gameObjectPtr->GetGraphicsComponent()->SetModel(vecModel[13]);
		gameObjectPtr->GetTransformComponent()->SetRotation(90.0f, 'y');
		bridges.push_back(gameObjectPtr);
		gameObjectVector[0]->AddChild(gameObjectPtr);
		
		
	}
	if (str == "back")
	{
		tempLoc = this->GetInputLocation();
		tempLoc.z = tempLoc.z - 1.f;
		gameObjectPtr = new GameObject;
		gameObjectPtr->SetActive();
		gameObjectPtr->SetTag("back");
		//gameObjectPtr->SetVelocity(15.0f);
		gameObjectPtr->AddComponent(std::make_shared<TransformComponent>());
		gameObjectPtr->AddComponent(std::make_shared<GraphicsComponent>());
		gameObjectPtr->GetTransformComponent()->SetLocation(tempLoc);
		gameObjectPtr->GetGraphicsComponent()->SetModel(vecModel[13]);
		gameObjectPtr->GetTransformComponent()->SetRotation(-90.0f, 'y');
		bridges.push_back(gameObjectPtr);
		gameObjectVector[0]->AddChild(gameObjectPtr);
	}
}

void Crossing::UpdateTransform()
{

}