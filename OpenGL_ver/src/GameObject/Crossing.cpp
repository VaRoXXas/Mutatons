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
float oneRotation = -90.f;
float twoRotation = 90.f;



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
		if (this->GetColliderComponent()->Collides(*posPtr, *sizePtr) && this->GetColliderComponent()->GetColliderTag()=="mainCollider")
		{
			if (gameObject->GetTag()[7] - '0' != count)
				count = gameObject->GetTag()[7] - '0';
			if ((dir == "right" && gameObject->GetDirection() == "forward") || (dir =="left" && gameObject->GetDirection() == "forward"))
			{
				//posPtr->z = posPtr->z - 0.001;
				posPtr->z = posPtr->z - 0.05f;
				gameObject->GetTransformComponent()->SetLocation(*posPtr);
			}
			else if ((dir == "right" && gameObject->GetDirection() == "back") || (dir == "left" && gameObject->GetDirection() == "back"))
			{
				//posPtr->z = posPtr->z + 0.001;
				posPtr->z = posPtr->z + 0.05f;
				gameObject->GetTransformComponent()->SetLocation(*posPtr);
			}
			else if ((dir == "forward" && gameObject->GetDirection() == "right") || (dir == "back" && gameObject->GetDirection() == "right"))
			{
				//posPtr->x = posPtr->x + 0.001;
				posPtr->x = posPtr->x + 0.05f;
				gameObject->GetTransformComponent()->SetLocation(*posPtr);
			}
			else if ((dir == "forward" && gameObject->GetDirection() == "left") || (dir == "back" && gameObject->GetDirection() == "left"))
			{
				//posPtr->x = posPtr->x - 0.001;
				posPtr->x = posPtr->x - 0.05f;
				gameObject->GetTransformComponent()->SetLocation(*posPtr);
			}
			gameObject->SetDirection(dir);
		}
	}
}

//This function interprets input from mouse
void Crossing::InputDirection()
{
	if (GetClick() && !once && !availableDirs.empty() && !blocked)
	{
		once = true;
		if (dir != availableDirs[0])
		{
			SetDir(availableDirs[0]);
			openedBridge = 0;
		}
		else if (dir == availableDirs[0])
		{
			SetDir(availableDirs[1]);
			openedBridge = 1;
		}
			
	}
	else
		once = false;
		
}

//Adds switch with arrows to the crossing
void Crossing::AddSwitch()
{
	directionSwitch = new GameObject;
	directionSwitch->SetActive();
	directionSwitch->AddComponent(std::make_shared<TransformComponent>(glm::vec3(0.f,0.f,0.f)));
	directionSwitch->AddComponent(std::make_shared<GraphicsComponent>());
	directionSwitch->GetGraphicsComponent()->SetModel(vecModel[30]);
	directionSwitch->GetTransformComponent()->SetRotation(180.0f, 'y');
	this->AddChild(directionSwitch);
}

//Dir setter, this function also sets
void Crossing::SetDir(std::string str)
{
	dir = str;

	if (!bridges.empty())
	{
		if (firstTime)
			twoRotation = 0.0f;
		else twoRotation = 90.f;

		if (bridges[0]->GetTag() == dir && bridges[1]->GetTag() == dir)
		{
			bridges[0]->GetTransformComponent()->SetRotation(twoRotation,'z');
			bridges[1]->GetTransformComponent()->SetRotation(twoRotation, 'z');

			bridges[2]->GetTransformComponent()->SetRotation(oneRotation, 'z');
			bridges[3]->GetTransformComponent()->SetRotation(oneRotation, 'z');
		}
		else if (bridges[2]->GetTag() == dir && bridges[3]->GetTag() == dir)
		{
			bridges[0]->GetTransformComponent()->SetRotation(oneRotation, 'z');
			bridges[1]->GetTransformComponent()->SetRotation(oneRotation, 'z');

			bridges[2]->GetTransformComponent()->SetRotation(twoRotation, 'z');
			bridges[3]->GetTransformComponent()->SetRotation(twoRotation, 'z');
		}

		if (dir == "left" && directionSwitch!=nullptr)
		{
			directionSwitch->GetTransformComponent()->SetRotation(-180.0f, 'y');
		}
		if (dir == "right" && directionSwitch != nullptr)
		{
			directionSwitch->GetTransformComponent()->SetRotation(180.0f, 'y');
		}
		if (dir == "forward" && directionSwitch != nullptr)
		{
			directionSwitch->GetTransformComponent()->SetRotation(-180.0f, 'y');
		}
		if (dir == "back" && directionSwitch != nullptr)
		{
			directionSwitch->GetTransformComponent()->SetRotation(180.0f, 'y');
		}

	}
	firstTime = false;
}

//adds directions to the list
void Crossing::AddDir(std::string str)
{
	tempLoc = glm::vec3(0.0f);
	availableDirs.push_back(str);
	if (str == "right")
	{
		tempLoc.x = -1.0f;
		gameObjectPtr = new GameObject;
		gameObjectPtr->SetActive();
		gameObjectPtr->SetTag("right");
		gameObjectPtr->AddComponent(std::make_shared<TransformComponent>());
		gameObjectPtr->AddComponent(std::make_shared<ColliderComponent>());
		gameObjectPtr->AddComponent(std::make_shared<GraphicsComponent>());
		gameObjectPtr->GetTransformComponent()->SetLocation(tempLoc);
		gameObjectPtr->GetColliderComponent()->Initialize(tempLoc,glm::vec3(1.0f));
		gameObjectPtr->GetGraphicsComponent()->SetModel(vecModel[36]);
		bridges.push_back(gameObjectPtr);
		this->AddChild(gameObjectPtr);
		tempLoc.x = -2.0f;
		gameObjectPtr = new GameObject;
		gameObjectPtr->SetActive();
		gameObjectPtr->SetTag("right");
		gameObjectPtr->AddComponent(std::make_shared<TransformComponent>());
		gameObjectPtr->AddComponent(std::make_shared<GraphicsComponent>());
		gameObjectPtr->GetTransformComponent()->SetLocation(tempLoc);
		gameObjectPtr->GetTransformComponent()->SetRotation(180.f,'y');
		gameObjectPtr->GetGraphicsComponent()->SetModel(vecModel[36]);
		bridges.push_back(gameObjectPtr);
		this->AddChild(gameObjectPtr);
		
		tempLoc = this->GetTransformComponent()->GetLocation();
		tempLoc.x = tempLoc.x -1.5f;
		bridgeOneCollider->Initialize(tempLoc, glm::vec3(0.7f, 1.0f, 1.0f));
		this->AddComponent(bridgeOneCollider);
	}
	if (str == "left")
	{
		tempLoc.x = 2.0f;
		gameObjectPtr = new GameObject;
		gameObjectPtr->SetActive();
		gameObjectPtr->SetTag("left");
		gameObjectPtr->AddComponent(std::make_shared<TransformComponent>());
		gameObjectPtr->AddComponent(std::make_shared<GraphicsComponent>());
		gameObjectPtr->GetTransformComponent()->SetLocation(tempLoc);
		gameObjectPtr->GetGraphicsComponent()->SetModel(vecModel[36]);
		bridges.push_back(gameObjectPtr);
		this->AddChild(gameObjectPtr);
		tempLoc.x = 1.0f;
		gameObjectPtr = new GameObject;
		gameObjectPtr->SetActive();
		gameObjectPtr->SetTag("left");
		gameObjectPtr->AddComponent(std::make_shared<TransformComponent>());
		gameObjectPtr->AddComponent(std::make_shared<GraphicsComponent>());
		gameObjectPtr->GetTransformComponent()->SetLocation(tempLoc);
		gameObjectPtr->GetTransformComponent()->SetRotation(180.f, 'y');
		gameObjectPtr->GetGraphicsComponent()->SetModel(vecModel[36]);
		bridges.push_back(gameObjectPtr);
		this->AddChild(gameObjectPtr);

		tempLoc = this->GetTransformComponent()->GetLocation();
		tempLoc.x = tempLoc.x + 1.5f;
		bridgeTwoCollider->Initialize(tempLoc, glm::vec3(0.7f,1.0f,1.0f));
		this->AddComponent(bridgeTwoCollider);
	}
	if (str == "forward")
	{
		tempLoc.x = 2.0f;
		gameObjectPtr = new GameObject;
		gameObjectPtr->SetActive();
		gameObjectPtr->SetTag("forward");
		gameObjectPtr->AddComponent(std::make_shared<TransformComponent>(tempLoc));
		gameObjectPtr->AddComponent(std::make_shared<GraphicsComponent>());
		gameObjectPtr->GetGraphicsComponent()->SetModel(vecModel[36]);
		bridges.push_back(gameObjectPtr);
		this->AddChild(gameObjectPtr);
		tempLoc.x = 1.0f;
		gameObjectPtr = new GameObject;
		gameObjectPtr->SetActive();
		gameObjectPtr->SetTag("forward");
		gameObjectPtr->AddComponent(std::make_shared<TransformComponent>(tempLoc));
		gameObjectPtr->AddComponent(std::make_shared<GraphicsComponent>());
		gameObjectPtr->GetTransformComponent()->SetRotation(180.f, 'y');
		gameObjectPtr->GetGraphicsComponent()->SetModel(vecModel[36]);
		bridges.push_back(gameObjectPtr);
		this->AddChild(gameObjectPtr);

		tempLoc = this->GetTransformComponent()->GetLocation();
		tempLoc.z = tempLoc.z + 1.5f;
		bridgeTwoCollider->Initialize(tempLoc, glm::vec3(1.0f, 1.0f, 0.7f));
		this->AddComponent(bridgeTwoCollider);
	}
	if (str == "back")
	{
		tempLoc.x = -1.0f;
		gameObjectPtr = new GameObject;
		gameObjectPtr->SetActive();
		gameObjectPtr->SetTag("back");
		gameObjectPtr->AddComponent(std::make_shared<TransformComponent>(tempLoc));
		gameObjectPtr->AddComponent(std::make_shared<GraphicsComponent>());
		gameObjectPtr->GetGraphicsComponent()->SetModel(vecModel[36]);
		bridges.push_back(gameObjectPtr);
		this->AddChild(gameObjectPtr);
		tempLoc.x = -2.0f;
		gameObjectPtr = new GameObject;
		gameObjectPtr->SetActive();
		gameObjectPtr->SetTag("back");
		gameObjectPtr->AddComponent(std::make_shared<TransformComponent>(tempLoc));
		gameObjectPtr->AddComponent(std::make_shared<GraphicsComponent>());
		gameObjectPtr->GetTransformComponent()->SetRotation(180.f, 'y');
		gameObjectPtr->GetGraphicsComponent()->SetModel(vecModel[36]);
		bridges.push_back(gameObjectPtr);
		this->AddChild(gameObjectPtr);

		tempLoc = this->GetTransformComponent()->GetLocation();
		tempLoc.z = tempLoc.z - 1.5f;
		bridgeOneCollider->Initialize(tempLoc, glm::vec3(1.0f, 1.0f, 0.7f));
		this->AddComponent(bridgeOneCollider);
	}
}

void Crossing::CheckIfBlocked(GameObject* mutaton)
{
	if (mutaton->GetTag()[7] - '0' == count)
	{
		mutatonPosPtr = mutaton->GetColliderComponent()->GetPosRef();
		mutatonSizePtr = mutaton->GetColliderComponent()->GetSizeRef();
		if (bridgeOneCollider->Collides(*mutatonPosPtr, *mutatonSizePtr) || bridgeTwoCollider->Collides(*mutatonPosPtr, *mutatonSizePtr))
		{
			blocked = true;
			colided = true;
		}
		else if ((!bridgeOneCollider->Collides(*mutatonPosPtr, *mutatonSizePtr) && !bridgeTwoCollider->Collides(*mutatonPosPtr, *mutatonSizePtr) && colided))
		{
			blocked = false;
			colided = false;
			count++;
		} 
		if(!mutaton->IsActive())
		{
			blocked = false;
			colided = false;
			count++;
		}
	}
	mutatonPosPtr = mutaton->GetColliderComponent()->GetPosRef();
	mutatonSizePtr = mutaton->GetColliderComponent()->GetSizeRef();
	if (mutaton->GetTag()[7] - '0' != count && (bridgeOneCollider->Collides(*mutatonPosPtr, *mutatonSizePtr) || bridgeTwoCollider->Collides(*mutatonPosPtr, *mutatonSizePtr)))
	{
		if (mutaton->GetDirection() == "forward")
			mutaton->SetDirection("back");
		if (mutaton->GetDirection() == "back")
			mutaton->SetDirection("forward");
		if (mutaton->GetDirection() == "right")
			mutaton->SetDirection("left");
		if (mutaton->GetDirection() == "left")
			mutaton->SetDirection("right");
	}
}