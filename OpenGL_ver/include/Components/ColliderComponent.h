#pragma once


class ColliderComponent : public Component
{
private:
	std::string type = "collider";
	std::shared_ptr<TransformComponent> transformComponentPtr = nullptr;
	glm::mat4* transform;
	glm::vec3 pos;
	glm::vec3 size;
	Collisions col;
public:
	std::string Component::GetType();
	void Component::DisableComponent();
	void Component::EnableComponent();
	bool Component::Enabled();

	glm::vec3 GetPos();
	glm::vec3 GetSize();
	
	void Initialize(std::shared_ptr<TransformComponent> componentPtr);
	void Collides(glm::vec3 &collidingObjectLocation, glm::vec3 &collidingObjectSize);
	//void Component::start(GameObjectSharer* gos, GameObject* self)
	//{
	//
	//}

};

