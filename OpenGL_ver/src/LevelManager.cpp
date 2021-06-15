#include "pch.h"
#include "Component.h"
#include "Collisions.h"
#include "Rendering/animation/Animation.h"
#include "Rendering/animation/Animator.h"
#include "Rendering/CustomDrawing.h"
#include "Components/GraphicsComponent.h"
#include "Components/TransformComponent.h"
#include "Components/ColliderComponent.h"
#include "GameObject/GameObject.h"
#include "GameObject/GameObjectBlueprint.h"
#include "GameObject/BlueprintObjectParser.h"
#include "GameObject/GameObjectFactory.h"
#include "GameObject/GameObjectLoader.h"
#include "GameObject/Crossing.h"
#include "GameObject/Building.h"
#include "LevelManager.h"


extern std::vector<GameObject*> modifiableGameObjectVector;
extern std::vector<GameObject*> gameObjectVector;
extern std::vector<Crossing*> crossingVector;
extern std::vector<Building*> buildingVector;
extern GameObject* gameObjectPtr;
extern Crossing* crossingPtr;
extern Building* buildingPtr;



//Parent GameObject declaration
void LevelManager::CreateParent()
{
	if (gameObjectVector.empty())
	{
		gameObjectPtr = new GameObject;
		gameObjectPtr->SetActive();
		gameObjectPtr->SetTag("Parent");
		gameObjectPtr->AddComponent(std::make_shared<TransformComponent>());
		gameObjectVector.push_back(gameObjectPtr);
	}
}

//Loads chosen scene
void LevelManager::LoadLevel(std::string level)
{
	mutatonCounter = 0;
	if (currScene != -1)
	{
		modifiableGameObjectVector.clear();
		crossingVector.clear();
		buildingVector.clear();
		gameObjectVector[0]->ClearChildren();
	}
	if (level == "mainmenu")
	{
		currScene = 0;
		CustomDrawing::DrawMainmenu();
	}
	if (level == "first")
	{
		currScene = 1;
		loader.LoadGameObjects("res/level1.txt", *gameObjectVector[0]);
		loader.LoadGameObjects("res/level_buildings1.txt", *gameObjectVector[0]);
	}
	if (level == "second")
	{
		currScene = 2;
		loader.LoadGameObjects("res/level2.txt", *gameObjectVector[0]);
		loader.LoadGameObjects("res/level_buildings2.txt", *gameObjectVector[0]);
	}
}

//void LevelManager::SpawnMutaton()
//{
//	gameObjectPtr = new GameObject;
//	gameObjectPtr->SetTag("mutaton" + std::to_string(mutatonCounter));
//	gameObjectPtr->SetActive();
//	gameObjectPtr->SetVelocity(4.0f);
//	gameObjectPtr->AddComponent(std::make_shared<TransformComponent>(glm::vec3(10.0f, 1.0f, -7.0f)));
//	gameObjectPtr->AddComponent(std::make_shared<GraphicsComponent>());
//	gameObjectPtr->AddComponent(std::make_shared<ColliderComponent>());
//	gameObjectPtr->SetUpdate();
//	gameObjectPtr->GetTransformComponent()->SetScale(*objectScalePtr);
//	gameObjectPtr->GetGraphicsComponent()->SetModel(vecAnimModel[1]);
//	gameObjectPtr->GetGraphicsComponent()->SetOversized(true);
//	gameObjectPtr->GetGraphicsComponent()->InitializeAnimation(ANIM_CREATURE_BASIC);
//	modifiableGameObjectVector.push_back(gameObjectPtr);
//	gameObjectVector[0]->AddChild(gameObjectPtr);
//	std::cout << gameObjectPtr->GetTag() << std::endl;
//	mutatonCounter++;
//}