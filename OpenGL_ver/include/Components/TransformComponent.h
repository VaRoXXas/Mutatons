#pragma once

class Component;



class TransformComponent : public Component {
private:
	const std::string m_Type = "transform";
	bool m_Enabled = false;
	glm::mat4 transform;
public:
	TransformComponent(glm::vec3 targetLocation);
	TransformComponent();
	/// TODO: GetLocation();GetScale();GetRotation()
	glm::mat4 GetTransform();
	void SetLocation(glm::vec3 &targetLocation);
	void SetRotation(float degrees, char rotationDirection);
	void SetScale(float targetScale);
	std::string Component::GetType();
	void Component::DisableComponent();
	void Component::EnableComponent();
	bool Component::Enabled();
	//void Component::Start(GameObjectSharer* gos, GameObject* self)
	//{}
};
