#pragma once

class Component;
class ColliderComponent;
class GraphicsComponent;
class TransformComponent;
class UpdateComponent;



class GameObject 
{
private:
	std::vector<std::shared_ptr<Component>> m_Components;
	std::vector<GameObject*> children;
	std::string tag;
	glm::mat4 gameObjectTransform;
	glm::vec3 inputLocation;
	std::string moveDirection = "forward";
	bool active = false;
	bool useUpdate = false;
	int graphicsComponentLocation = -1;
	bool hasGraphicsComponent = false;
	bool hasTransformComponent = false;
	int transformComponentLocation = -1;
	int numberRectColliderComponents = 0;
	int colliderComponentLocation = -1;
	bool hasCollider = false;
	float velocity = 0.0f;
	float tempVel = 0.0f;
	bool hasInput = false;
	bool clickedOn = false;
	bool mouseHoveredOver = false;

	int element = 0;
public:
	void Update(glm::vec3& locationVec, GLfloat time);
	void AddComponent(std::shared_ptr<Component> component);
	void SetActive();
	void SetInactive();
	bool IsActive();
	void Destroy();
	void SetTag(std::string tagTyped);
	std::string GetTag();
	void AddChild(GameObject* childPtr);
	void Render(const glm::mat4& transform);
	void DepthRender();
	void SetDirection(std::string dir);
	std::string GetDirection();
	void SetUpdate();
	void SetVelocity(float vel);
	void SetElement(int elem);
	int GetElement();
	std::shared_ptr<Component> GetComponentByType(std::string type);
	bool HasCollider();
	bool HasUpdate();
	void SetInput(glm::vec3 loc);
	void CheckInput(glm::vec3& terrainPoint);
	bool GetHasInput();
	bool GetClick();
	bool HasMouseHoveringOver();
	glm::vec3 GetInputLocation();
	std::shared_ptr<GraphicsComponent> GetGraphicsComponent();
	std::shared_ptr<TransformComponent> GetTransformComponent();
	std::shared_ptr<ColliderComponent> GetColliderComponent();

};

