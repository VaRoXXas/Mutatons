#pragma once
#include "Component.h"



// WORK IN PROGRESS
class ColliderComponent : public Component
{
private:
	std::string type = "collider";
	bool enabled = false;

public:
	std::string Component::GetType()
	{
		return ColliderComponent::type;
	}
	void Component::DisableComponent()
	{
		enabled = false;
	}

	void Component::EnableComponent()
	{
		enabled = true;
	}

	bool Component::Enabled()
	{
		return enabled;
	}

	//void Component::start(GameObjectSharer* gos, GameObject* self)
	//{
	//
	//}

};

