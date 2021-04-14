#pragma once
#include "Component.h"

class Component;

class TransformComponent : public Component {
private:
	const std::string m_Type = "transform";
	glm::mat4 transform;
public:
	TransformComponent(glm::vec3 targetLocation);
	TransformComponent();
	/// TODO: GetLocation();GetScale();GetRotation()
	glm::mat4 GetTransform()
	{
		return transform;
	}
	void SetLocation(glm::vec3 targetLocation);
	void SetRotation(glm::vec3 targetRotation);
	void SetScale(glm::vec3 targetScale);
	std::string Component::GetType()
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
