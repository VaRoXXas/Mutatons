#include "pch.h"
#include "GameObject/GameObjectBlueprint.h"



//Getters and setters for all the propeties GameObject can have
float GameObjectBlueprint::GetX()
{
	return x;
}

void GameObjectBlueprint::SetX(float xFloat)
{
	x = xFloat;
}

float GameObjectBlueprint::GetY()
{
	return y;
}

void GameObjectBlueprint::SetY(float yFloat)
{
	y = yFloat;
}

float GameObjectBlueprint::GetZ()
{
	return z;
}

void GameObjectBlueprint::SetZ(float zFloat)
{
	z = zFloat;
}

float GameObjectBlueprint::GetScale()
{
	return scale;
}

void GameObjectBlueprint::SetScale(float scaleFloat)
{
	scale = scaleFloat;
}

float GameObjectBlueprint::GetRotation()
{
	return rotation;
}

void GameObjectBlueprint::SetRotation(float rotationFloat)
{
	rotation = rotationFloat;
}

char GameObjectBlueprint::GetRotationAxis()
{
	return rotationAxis;
}

void GameObjectBlueprint::SetRotationAxis(char rotation)
{
	rotationAxis = rotation;
}

void GameObjectBlueprint::SetTag(std::string tagString)
{
	tag = tagString;
}

std::string GameObjectBlueprint::GetTag()
{
	return tag;
}

std::vector<std::string>& GameObjectBlueprint::GetComponentList()
{
	return componentList;
}

void GameObjectBlueprint::AddToComponentList(std::string newComponent)
{
	componentList.push_back(newComponent);
}

int GameObjectBlueprint::GetModelID()
{
	return modelID;
}

void GameObjectBlueprint::SetModelID(int id)
{
	modelID = id;
}