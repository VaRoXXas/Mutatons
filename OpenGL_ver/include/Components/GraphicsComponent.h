#pragma once
#include "Component.h"

//class Component;
class Shader;
class Model;



class GraphicsComponent : public Component {
private:
	std::string m_Type = "graphics";
	bool m_Enabled = false;
	Model* modelPtr = nullptr;
public:
	GraphicsComponent();
	//void Draw(std::shared_ptr<TransformComponent> t);
	void Draw(Shader& shader, const glm::mat4& transform);
	void SetModel(Model* modelLoad);
	void Render(const glm::mat4& parentTransform);
	std::string Component::GetType();
	void Component::DisableComponent();
	void Component::EnableComponent();
	bool Component::Enabled();



	//void Component::start(
	//	GameObjectSharer* gos, GameObject* self) {}
};
