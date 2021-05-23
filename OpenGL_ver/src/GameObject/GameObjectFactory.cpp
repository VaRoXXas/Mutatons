#include "pch.h"
#include "GameObject/GameObject.h"
#include "GameObject/GameObjectBlueprint.h"
#include "Component.h"
#include "Components/TransformComponent.h"
#include "Rendering/animation/Animation.h"
#include "Rendering/animation/Animator.h"
#include "Components/GraphicsComponent.h"
#include "Collisions.h"
#include "Components/ColliderComponent.h"
#include "Rendering/Model.h"
#include "GameObject/GameObjectFactory.h"

extern std::vector<Model*> vecModel;



//this class creates new game object based on file input and adds it to the designated parent and game objects' list
void GameObjectFactory::BuildGameObjects(GameObjectBlueprint& bp, std::vector<GameObject*> gameObjects, GameObject& parentGameObject)
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
			gameObjectPtr->GetColliderComponent()->Initialize(gameObjectPtr->GetTransformComponent());
		}
	}

	parentGameObject.AddChild(gameObjectPtr);
	gameObjects.push_back(gameObjectPtr);
}