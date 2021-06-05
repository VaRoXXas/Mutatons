#pragma once


class GameObjectFactory
{
public:
	void BuildGameObjects(GameObjectBlueprint& bp, std::string gameObjectType, GameObject& parentGameObject);
};

