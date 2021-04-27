#pragma once
#include "Component.h"

class Shader;
class Model;



class GraphicsComponent : public Component {
private:
	std::string type = "graphics";
	Model* modelPtr = nullptr;
public:
	void SetModel(Model* modelLoad);
	void Render(const glm::mat4& parentTransform);
	std::string Component::GetType();
	void Component::DisableComponent();
	void Component::EnableComponent();
	bool Component::Enabled();
protected:


	//void Component::start(
	//	GameObjectSharer* gos, GameObject* self) {}
};
