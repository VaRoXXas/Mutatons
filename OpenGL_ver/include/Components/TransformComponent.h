#pragma once

class Component;



class TransformComponent : public Component {
private:
	const std::string type = "transform";
	glm::mat4 transform;
	glm::vec3 location;
	glm::vec3 scale;
public:
	TransformComponent(glm::vec3 targetLocation);
	TransformComponent();
	/// TODO: GetLocation();GetScale();GetRotation()
	glm::mat4 GetTransform();
	glm::vec3 GetLocation();
	glm::vec3 &GetLocationAddr();
	glm::vec3 GetScale();
	void SetLocation(glm::vec3 &targetLocation);
	void SetRotation(float degrees, char rotationDirection);
	void SetScale(float &targetScale);
	void SetScale(glm::vec3 &targetScale);
	std::string Component::GetType();
	void Component::DisableComponent();
	void Component::EnableComponent();
	bool Component::Enabled();
	//void Component::Start(GameObjectSharer* gos, GameObject* self)
	//{}
};
