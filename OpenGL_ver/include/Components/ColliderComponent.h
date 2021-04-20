#pragma once


class ColliderComponent : public Component
{
private:
	std::string type = "collider";
	std::shared_ptr<TransformComponent> transformComponentPtr = nullptr;
	glm::mat4* transform;
	glm::vec3 pos;
	glm::vec3 size;
public:
	std::string Component::GetType();
	void Component::DisableComponent();
	void Component::EnableComponent();
	bool Component::Enabled();

	glm::vec3 GetPos();
	glm::vec3 GetSize();

	Collisions col;
	
	void SetTransformPtr(std::shared_ptr<TransformComponent> componentPtr);
	void CollisionCalc();
	//void Component::start(GameObjectSharer* gos, GameObject* self)
	//{
	//
	//}

};

