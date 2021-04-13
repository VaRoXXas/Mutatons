#pragma once
#include "TransformComponent.h"

class Shader;
class Model;
class Component;

class GraphicsComponent : public Component {
private:
	std::string m_Type = "graphics";
	bool m_Enabled = false;
	Model* modelPtr = nullptr;
public:
	GraphicsComponent();
	//void Draw(std::shared_ptr<TransformComponent> t);
	void Draw(Shader& shader, const glm::mat4& transform);
	void LoadModel(Model* modelLoad);
	void Render(const glm::mat4& parentTransform);
	std::string Component::GetType() {
		return m_Type;
	}
	std::string Component::GetSpecificType()
	{
		return m_Type;
	}
	void Component::DisableComponent() {
		m_Enabled = false;
	}
	void Component::EnableComponent() {
		m_Enabled = true;
	}
	bool Component::Enabled() {
		return m_Enabled;
	}



	//void Component::start(
	//	GameObjectSharer* gos, GameObject* self) {}
};