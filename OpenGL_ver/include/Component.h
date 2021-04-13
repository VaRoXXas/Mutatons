#pragma once

//#include "GameObjectSharer.h"
#include "pch.h"



class GameObject;

class Component 
{
public:
	virtual std::string GetType() = 0;
	virtual std::string GetSpecificType() = 0;
	virtual void DisableComponent() = 0;
	virtual void EnableComponent() = 0;
	virtual bool Enabled() = 0;
	//virtual void Start(GameObjectSharer* gos, GameObject* self) = 0;

};
