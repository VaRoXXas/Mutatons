#pragma once



class GameObjectLoader
{
private:
	BlueprintObjectParser parser;
	GameObjectFactory gameObjectFactory;
public:
	void LoadGameObjects(std::string pathToFile, std::vector<GameObject*> gameObjects, GameObject& parentGameObject);
};

