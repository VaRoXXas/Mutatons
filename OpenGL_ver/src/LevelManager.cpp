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
	if (level == "lose")
	{
		currScene = 1;
		CustomDrawing::DrawLose();
	}
	if (level == "victory")
	{
		currScene = 2;
		CustomDrawing::DrawVictory();
	}
	if (level == "tutorialScreen")
	{
		currScene = 3;
		CustomDrawing::DrawTutorial();
	}
	if (level == "first")
	{
		//level settings
		//maxCapturedPoints = 1;
		//capturedCounter = 0;
		//counter = 0;
		//mutatonCounter = 0;
		//maxMutatonsInLevel = 3;
		//mutatonsInControl = 0;
		//capturedPoints.clear();
		//spawnPointPtr = new glm::vec3(5.0f, 1.0f, -6.0f);

		currScene = 11;
		loader.LoadGameObjects("res/level1.txt", *gameObjectVector[0]);
		loader.LoadGameObjects("res/level_buildings1.txt", *gameObjectVector[0]);
	}
	if (level == "second")
	{
		//level settings
		//maxCapturedPoints = 2;
		//capturedCounter = 0;
		//counter = 0;
		//mutatonCounter = 0;
		//maxMutatonsInLevel = 8;
		//mutatonsInControl = 0;
		//capturedPoints.clear();
		//spawnPointPtr = new glm::vec3(10.0f, 1.0f, -7.0f);

		currScene = 12;
		loader.LoadGameObjects("res/level2.txt", *gameObjectVector[0]);
		loader.LoadGameObjects("res/level_buildings2.txt", *gameObjectVector[0]);
	}
	if (level == "tutorial")
	{
		//level settings
		//maxCapturedPoints = 1;
		//capturedCounter = 0;
		//counter = 0;
		//mutatonCounter = 0;
		//maxMutatonsInLevel = 3;
		//mutatonsInControl = 0;
		//capturedPoints.clear();
		//spawnPointPtr = new glm::vec3(10.0f, 1.0f, -7.0f);

		currScene = 20;
		loader.LoadGameObjects("res/level_tutorial.txt", *gameObjectVector[0]);
		loader.LoadGameObjects("res/level_buildings_tutorial.txt", *gameObjectVector[0]);
	}
}
int LevelManager::GetCurrScene()
{
	return currScene;
}
void LevelManager::SetCurrScene(int scene)
{
	currScene = scene;
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