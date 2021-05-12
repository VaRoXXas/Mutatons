#pragma once
#include "Component.h"

class Shader;
class Model;



class GraphicsComponent : public Component {
private:
	std::string type = "graphics";
	Model* modelPtr = nullptr;
	bool animated = 0;
	Animation* animPtr;
	Animator* animatorPtr;

public:
	void SetModel(Model* modelLoad);
	void InitializeAnimation(const std::string& path);
	void Render(const glm::mat4& parentTransform);
	void DepthRender(const glm::mat4& parentTransform);
	bool GetAnimated();
	Animator* GetAnimator();
	std::string Component::GetType();
	void Component::DisableComponent();
	void Component::EnableComponent();
	bool Component::Enabled();
protected:


	//void Component::start(
	//	GameObjectSharer* gos, GameObject* self) {}
};
