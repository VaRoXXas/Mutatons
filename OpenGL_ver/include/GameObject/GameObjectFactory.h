#pragma once


class GameObjectFactory
{
public:
	void BuildGameObjects(GameObjectBlueprint& bp, std::vector <GameObject*> gameObjects, GameObject& parentGameObject);
};

