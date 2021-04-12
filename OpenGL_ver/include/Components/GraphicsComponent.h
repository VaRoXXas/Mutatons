#pragma once
#include "TransformComponent.h"
#include "Rendering/deprecated/Model.h"

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

	void LoadModel(Model* modelLoad);

	//void Component::start(
	//	GameObjectSharer* gos, GameObject* self) {}
};