#pragma once
#include "TransformComponent.h"

class GraphicsComponent : public Component {
private:
	std::string m_Type = "graphics";
	bool m_Enabled = false;
public:
	virtual void Draw(GLFWwindow& window, std::shared_ptr<TransformComponent> t) = 0;
	virtual void InitializeGraphics(std::string textureName) = 0;

	std::string Component::GetType() {
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