#pragma once
#include "Component.h"

class Component;

class TransformComponent : public Component {
private:
	const std::string m_Type = "transform";
	glm::mat4 transform;
	glm::mat4 scale;
	glm::mat4 rotation;
public:
	TransformComponent(glm::mat4 transform, glm::mat4 scale, glm::mat4 rotation);
	/// TODO: GetLocation();GetScale();GetRotation()
	glm::mat4 GetLocation()
	{
		return transform;
	}

	std::string Component::GetType()
	{
		return m_Type;
	}
	std::string Component::GetSpecificType()
	{
		return m_Type;
	}
	void Component::DisableComponent() {}
	void Component::EnableComponent() {}
	bool Component::Enabled()
	{
		return false;
	}
	//void Component::Start(GameObjectSharer* gos, GameObject* self)
	//{}
};
