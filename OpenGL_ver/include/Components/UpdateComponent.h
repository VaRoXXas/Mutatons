#pragma once



class TransformComponent;
class UpdateComponent : public Component
{
private:
	std::string m_Type = "update";
	bool m_Enabled = false;
	int objectBehavior = 0;
	std::shared_ptr<TransformComponent> transformComponentPtr;
public:
	void Update(float fps, glm::mat4 *transform);
	void SetBehavior(std::string chooseOne);
	//void Move();
	std::string Component::GetType();
	void Component::DisableComponent();
	void Component::EnableComponent();
	bool Component::Enabled();

	//void Component::Start(
	//	GameObjectSharer* gos, GameObject* self) {
	//}
};
