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
	std::string colliderTag;
public:
	std::string Component::GetType();
	void Component::DisableComponent();
	void Component::EnableComponent();
	bool Component::Enabled();
	void SetColliderTag(std::string typeTag);
	std::string GetColliderTag();

	glm::vec3 GetPos();
	glm::vec3 GetSize();
	glm::vec3* GetPosRef();
	glm::vec3* GetSizeRef();
	
	void Initialize(std::shared_ptr<TransformComponent> componentPtr);
	void Initialize(glm::vec3& targetLocation, glm::vec3& targetSize);
	void Initialize(glm::vec3 targetLocation, glm::vec3 targetSize);
	bool Collides(glm::vec3 &collidingObjectLocation, glm::vec3 &collidingObjectSize);
	void Update();

};

