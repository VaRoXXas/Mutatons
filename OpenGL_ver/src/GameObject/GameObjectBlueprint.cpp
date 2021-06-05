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

float GameObjectBlueprint::GetColX()
{
	return colX;
}

void GameObjectBlueprint::SetColX(float xFloat)
{
	colX = xFloat;
}

float GameObjectBlueprint::GetColY()
{
	return colY;
}

void GameObjectBlueprint::SetColY(float yFloat)
{
	colY = yFloat;
}

float GameObjectBlueprint::GetColZ()
{
	return colZ;
}

void GameObjectBlueprint::SetColZ(float zFloat)
{
	colZ = zFloat;
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

void GameObjectBlueprint::SetInput(std::string ifInput)
{
	if (ifInput == "YES")
		hasInput = 1;
}

bool GameObjectBlueprint::GetInput()
{
	return hasInput;
}

//crossings setters

void GameObjectBlueprint::AddDir(std::string direction)
{
	dirs.push_back(direction);
}

std::vector<std::string>& GameObjectBlueprint::GetDirs()
{
	return dirs;
}

void GameObjectBlueprint::SetCurDir(std::string direction)
{
	currentDir = direction;
}

std::string GameObjectBlueprint::GetCurrentDir()
{
	return currentDir;
}
 
void GameObjectBlueprint::SetSwitch(std::string switchString)
{
	if (switchString == "YES")
		ifSwitch = 1;
	else
		ifSwitch = 0;
}

bool GameObjectBlueprint::GetIfSwitch()
{
	return ifSwitch;
}