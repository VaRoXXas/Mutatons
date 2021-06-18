#include "pch.h"
#include "GameObject/GameObject.h"
#include "GameObject/Crossing.h"
#include "GameObject/Building.h"
#include "GameObject/GameObjectBlueprint.h"
#include "Component.h"
#include "Components/TransformComponent.h"
#include "Rendering/animation/Animation.h"
#include "Rendering/animation/Animator.h"
#include "Components/GraphicsComponent.h"
#include "Collisions.h"
#include "Components/ColliderComponent.h"
#include "Rendering/Model.h"
#include "GameObject/BlueprintObjectParser.h"
#include "GameObject/GameObjectFactory.h"
#include "GameObject/GameObjectLoader.h"

extern std::vector<Model*> vecModel;
extern std::vector<GameObject*> gameObjectVector;
extern std::vector<Crossing*> crossingVector;
extern std::vector<Building*> buildingVector;



//this class creates new game object based on file input and adds it to the designated parent and game objects' list
void GameObjectFactory::BuildGameObjects(GameObjectBlueprint& bp, std::string gameObjectType, GameObject& parentGameObject)
{
	//GameObject* gameObjectPtr = new Crossing;
	if (gameObjectType == "crossing")
	{
		Crossing* gameObjectPtr = new Crossing;

		gameObjectPtr->SetActive();
		auto it = bp.GetComponentList().begin();
		auto end = bp.GetComponentList().end();
		for (it; it != end; ++it)
		{
			if (*it == "Transform")
			{
				//float tempScale = bp.GetScale();
				glm::vec3 tempScale = glm::vec3(bp.GetScale(), bp.GetScale(), bp.GetScale());
				gameObjectPtr->AddComponent(std::make_shared<TransformComponent>(glm::vec3(bp.GetX(), bp.GetY(), bp.GetZ())));
				gameObjectPtr->GetTransformComponent()->SetRotation(bp.GetRotation(), bp.GetRotationAxis());
				if (bp.GetRotation() == 90 || bp.GetRotation() == 180)
				{
					tempScale.x = -tempScale.x;
				}
				gameObjectPtr->GetTransformComponent()->SetScale(tempScale);
			}
			else if (*it == "Graphics")
			{
				gameObjectPtr->AddComponent(std::make_shared<GraphicsComponent>());
				gameObjectPtr->GetGraphicsComponent()->SetModel(vecModel[bp.GetModelID()]);
			}
			else if (*it == "Collider")
			{
				//glm::vec3 tempLoc = this->GetTransformComponent()->GetLocation();
				//tempLoc.z = tempLoc.z + 0.9f;
				//tempLoc.x = tempLoc.x + 0.9f;
				//this->GetColliderComponent()->Initialize(tempLoc, glm::vec3(0.1f));
				std::shared_ptr<ColliderComponent> collider = std::make_shared<ColliderComponent>();
				collider->SetColliderTag("mainCollider");
				collider->Initialize(glm::vec3(bp.GetColX() + 0.05f, bp.GetColY(), bp.GetColZ() + 0.05f), glm::vec3(0.9f, 0.9f, 0.9f));
				gameObjectPtr->AddComponent(collider);
				//gameObjectPtr->GetColliderComponent()->Initialize(glm::vec3(bp.GetColX(), bp.GetColY(), bp.GetColZ()), glm::vec3(1.0f, 1.0f, 1.0f));
			}
		}
		for (std::string di : bp.GetDirs())
			gameObjectPtr->AddDir(di);
		gameObjectPtr->SetDir(bp.GetCurrentDir());
		if (bp.GetIfSwitch())
			gameObjectPtr->AddSwitch();
		gameObjectPtr->SetTag(bp.GetTag());
		if (bp.GetInput())
			gameObjectPtr->SetInput(gameObjectPtr->GetTransformComponent()->GetLocation());
		parentGameObject.AddChild(gameObjectPtr);
		crossingVector.push_back(gameObjectPtr);
	}
		
	if (gameObjectType == "object")
	{
		GameObject* gameObjectPtr = new GameObject;

		gameObjectPtr->SetTag(bp.GetTag());
		gameObjectPtr->SetActive();
		auto it = bp.GetComponentList().begin();
		auto end = bp.GetComponentList().end();
		for (it; it != end; ++it)
		{
			if (*it == "Transform")
			{
				float tempScale = bp.GetScale();
				gameObjectPtr->AddComponent(std::make_shared<TransformComponent>(glm::vec3(bp.GetX(), bp.GetY(), bp.GetZ())));
				gameObjectPtr->GetTransformComponent()->SetRotation(bp.GetRotation(), bp.GetRotationAxis());
				gameObjectPtr->GetTransformComponent()->SetScale(tempScale);
			}
			else if (*it == "Graphics")
			{
				gameObjectPtr->AddComponent(std::make_shared<GraphicsComponent>());
				gameObjectPtr->GetGraphicsComponent()->SetModel(vecModel[bp.GetModelID()]);
			}
			else if (*it == "Collider")
			{
				gameObjectPtr->AddComponent(std::make_shared<ColliderComponent>());
				gameObjectPtr->GetColliderComponent()->Initialize(glm::vec3(bp.GetColX(), bp.GetColY(), bp.GetColZ()), glm::vec3(1.0f));
			}
		}
		if (bp.GetInput())
			gameObjectPtr->SetInput(gameObjectPtr->GetTransformComponent()->GetLocation());
		parentGameObject.AddChild(gameObjectPtr);
		gameObjectVector.push_back(gameObjectPtr);
	}

	if (gameObjectType == "building")
	{
		Building* gameObjectPtr = new Building;
		gameObjectPtr->SetTag(bp.GetTag());
		gameObjectPtr->SetActive();
		auto it = bp.GetComponentList().begin();
		auto end = bp.GetComponentList().end();
		for (it; it != end; ++it)
		{
			if (*it == "Transform")
			{
				float tempScale = bp.GetScale();
				gameObjectPtr->AddComponent(std::make_shared<TransformComponent>(glm::vec3(bp.GetX(), bp.GetY(), bp.GetZ())));
				gameObjectPtr->GetTransformComponent()->SetRotation(bp.GetRotation(), bp.GetRotationAxis());
				gameObjectPtr->GetTransformComponent()->SetScale(tempScale);
			}
			else if (*it == "Graphics")
			{
				gameObjectPtr->AddComponent(std::make_shared<GraphicsComponent>());
				//gameObjectPtr->GetGraphicsComponent()->SetModel(vecModel[bp.GetModelID()]);
			}
			else if (*it == "Collider")
			{
				gameObjectPtr->AddComponent(std::make_shared<ColliderComponent>());
				gameObjectPtr->GetColliderComponent()->Initialize(glm::vec3(bp.GetColX(), bp.GetColY(), bp.GetColZ()), glm::vec3(1.0f));
			}
		}
		if (bp.GetInput())
			gameObjectPtr->SetInput(gameObjectPtr->GetTransformComponent()->GetLocation());

		gameObjectPtr->SetElement(bp.GetElement());
		gameObjectPtr->SetType(bp.GetType());
		if (gameObjectPtr->GetType() == "Control")
		{
			for (std::string ob : bp.GetObstacles())
			{
				for (Building* b : buildingVector)
				{
					if (b->GetTag() == ob)
						gameObjectPtr->AddChildObstacle(b);
				}
			}
		}

		if (gameObjectPtr->GetType() == "Obstacle")
		{
			for (std::string ls : bp.GetLaserSides())
			{
				gameObjectPtr->AddLaserSide(ls);
			}
		}

		parentGameObject.AddChild(gameObjectPtr);
		buildingVector.push_back(gameObjectPtr);
	}
}