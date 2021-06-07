#include "pch.h"
#include "GameObject/GameObject.h"
#include "GameObject/Crossing.h"
#include "GameObject/Building.h"
#include "ObjectTags.h"
#include "GameObject/GameObjectBlueprint.h"
#include "GameObject/BlueprintObjectParser.h"
#include "GameObject/GameObjectFactory.h"
#include "GameObject/GameObjectLoader.h"

extern std::vector<GameObject*> gameObjectVector;
extern std::vector<Crossing*> crossingVector;
extern std::vector<Building*> buildingVector;

//this function calls the beginning of gameobject declaration in given file, and later calls building of read game objects
void GameObjectLoader::LoadGameObjects(std::string pathToFile, GameObject& parentGameObject)
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
			gameObjectFactory.BuildGameObjects(bp, "object", parentGameObject);
		}
		if (lineFromFile.find(ObjectTags::START_OF_CROSSING) != std::string::npos)
		{
			GameObjectBlueprint bp;
			parser.ParseNextObjectForBlueprint(reader, bp);
			gameObjectFactory.BuildGameObjects(bp, "crossing", parentGameObject);
		}
		if (lineFromFile.find(ObjectTags::START_OF_BUILDING) != std::string::npos)
		{
			GameObjectBlueprint bp;
			parser.ParseNextObjectForBlueprint(reader, bp);
			gameObjectFactory.BuildGameObjects(bp, "building", parentGameObject);
		}
	}
}