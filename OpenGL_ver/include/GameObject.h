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
public:
	void Update(glm::vec3 &locationVec);
	void AddComponent(std::shared_ptr<Component> component);
	void SetActive();
	void SetInactive();
	bool IsActive();
	void SetTag(std::string tag);
	std::string GetTag();
	void AddChild(GameObject* childPtr);
	void Render();
	void SetDirection(std::string dir);
	void SetUpdate();
	//void Start(GameObjectSharer* gos);
	// Slow only use in init and start
	std::shared_ptr<Component> GetComponentByType(std::string type);
	//FloatRect& GetEncompassingRectCollider();
	bool HasCollider();
	bool HasUpdate();
	//std::string GetEncompassingRectColliderTag();
	std::shared_ptr<GraphicsComponent> GetGraphicsComponent();
	std::shared_ptr<TransformComponent> GetTransformComponent();
	std::shared_ptr<ColliderComponent> GetColliderComponent();
};

/*
GameObject declaration:
GameObject gameobject
gameobject.addComponent(make_shared<ComponentName>())
e.g.
gameObject.addComponent(make_shared<TransformComponent>(transform,scale,rotation));
<-------->


#include "Component.h"
#include "Components/GraphicsComponent.h"
#include "DataManager.h"
#include "GameObject.h"
#include "Components/UpdateComponent.h"
#include "Components/TransformComponent.h"

extern Model* modelPtr;

DataManager dataManager;
dataManager.LoadAllModels();

GameObject* gameObjectPtr;
GameObject objekt;
objekt.SetTag("PierwszyGameObject");
std::shared_ptr<GraphicsComponent> gp = std::make_shared<GraphicsComponent>();
objekt.AddComponent(gp);

gameObjectPtr = &objekt;


gp->SetModel(modelPtr);
gameObjectPtr->Render();
*/
