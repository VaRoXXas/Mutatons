#include "pch.h"
#include "GameObject.h"
#include "ObjectTags.h"
#include "GameObjectBlueprint.h"
#include "BlueprintObjectParser.h"
#include "GameObjectFactory.h"
#include "GameObjectLoader.h"



//this function calls the beginning of gameobject declaration in given file, and later calls building of read game objects
void GameObjectLoader::LoadGameObjects(std::string pathToFile, std::vector<GameObject*> gameObjects, GameObject& parentGameObject)
{
	std::ifstream reader(pathToFile);
	std::string lineFromFile;
	float x = 0, y = 0, width = 0, height = 0;
	std::string value = "";
	while (getline(reader, lineFromFile)) 
	{
		if (lineFromFile.find(ObjectTags::START_OF_OBJECT) != std::string::npos) 
		{
			GameObjectBlueprint bp;
			parser.ParseNextObjectForBlueprint(reader, bp);
			gameObjectFactory.BuildGameObjects(bp, gameObjects, parentGameObject);
		}
	}
}