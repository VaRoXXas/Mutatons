#pragma once



class GameObjectLoader
{
private:
	BlueprintObjectParser parser;
	GameObjectFactory gameObjectFactory;
public:
	void LoadGameObjects(std::string pathToFile, GameObject& parentGameObject);
};

