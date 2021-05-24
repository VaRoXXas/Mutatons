#pragma once
#include "Component.h"

class Shader;
class Model;
class GameObject;


class GraphicsComponent : public Component {
private:
	std::string type = "graphics";
	Model* modelPtr = nullptr;
	bool animated = 0;
	Animation* animPtr;
	Animator* animatorPtr;
	bool isHighlighted = false; // strange method of determining, because there is no defined way to access parent game object of this component from within it

public:
	void SetModel(Model* modelLoad);
	void SetHighlighted(bool value);
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
