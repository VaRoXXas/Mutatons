#pragma once
#include "Component.h"

class ColliderComponent : public Component
{
private:
	std::string m_Type = "collider";
	bool m_Enabled = false;

public:
	std::string Component::GetType()
	{
		return ColliderComponent::m_Type;
	}
	void Component::DisableComponent()
	{
		m_Enabled = false;
	}

	void Component::EnableComponent()
	{
		m_Enabled = true;
	}

	bool Component::Enabled()
	{
		return m_Enabled;
	}

	//void Component::start(GameObjectSharer* gos, GameObject* self)
	//{
	//
	//}

};

