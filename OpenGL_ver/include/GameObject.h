#pragma once
#include "Components/UpdateComponent.h"

class GameObject 
{
private:
	std::vector<std::shared_ptr<Component>> m_Components;
	
	std::string m_Tag;
	bool m_Active = false;
	int m_NumberUpdateComponents = 0;
	bool m_HasUpdateComponent = false;
	int m_FirstUpdateComponentLocation = -1;
	int m_GraphicsComponentLocation = -1;
	bool m_HasGraphicsComponent = false;
	int m_TransformComponentLocation = -1;
	int m_NumberRectColliderComponents = 0;
	int m_FirstRectColliderComponentLocation = -1;
	bool m_HasCollider = false;
public:
	void Update(float fps);
	//void Draw(GLFWwindow& window);
	void AddComponent(std::shared_ptr<Component> component);
	void SetActive();
	void SetInactive();
	bool IsActive();
	void SetTag(std::string tag);
	std::string GetTag();
	//void Start(GameObjectSharer* gos);
	// Slow only use in init and start
	std::shared_ptr<Component> GetComponentByTypeAndSpecificType(std::string type, std::string specificType);
	//FloatRect& GetEncompassingRectCollider();
	bool HasCollider();
	bool HasUpdateComponent();
	//std::string GetEncompassingRectColliderTag();
	std::shared_ptr<GraphicsComponent> GetGraphicsComponent();
	std::shared_ptr<TransformComponent> GetTransformComponent();
	std::shared_ptr<UpdateComponent> GetFirstUpdateComponent();
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


gp->LoadModel(modelPtr);
gp->Render(transform);
*/