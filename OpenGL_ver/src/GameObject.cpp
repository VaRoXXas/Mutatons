#include "pch.h"
#include "GameObject.h"
#include "Component.h"
#include "Collisions.h"
#include "Components/ColliderComponent.h"
#include "Components/GraphicsComponent.h"
#include "Components/TransformComponent.h"

//#include "GameObjectSharer.h"
//#include "DevelopState.h"
//#include "RectColliderComponent.h"



//Allows GameObject to move in four directions
void GameObject::Update(glm::vec3& locationVec)
{
	if (active && useUpdate)
	{
		this->GetTransformComponent()->SetLocation(locationVec);
		if (moveDirection == "forward")
		{
			locationVec.z = locationVec.z + 0.0001f * velocity;
		}
		else if (moveDirection == "back")
		{
			locationVec.z = locationVec.z - 0.0001f * velocity;
		}
		else if (moveDirection == "right")
		{
			locationVec.x = locationVec.x - 0.0001f * velocity;
		}
		else if (moveDirection == "left")
		{
			locationVec.x = locationVec.x + 0.0001f * velocity;
		}
		else if (moveDirection == "up")
		{
			locationVec.y = locationVec.y + 0.0001f * velocity;
		}
		else if (moveDirection == "down")
		{
			locationVec.y = locationVec.y - 0.0001f * velocity;
		}
	}
}

//GameObject's Components' getters
std::shared_ptr<GraphicsComponent> GameObject::GetGraphicsComponent()
{
	return std::static_pointer_cast<GraphicsComponent>(
		m_Components[graphicsComponentLocation]);
}

std::shared_ptr<TransformComponent> GameObject::GetTransformComponent()
{
	return std::static_pointer_cast<TransformComponent>(
		m_Components[transformComponentLocation]);
}
std::shared_ptr<ColliderComponent> GameObject::GetColliderComponent()
{
	return std::static_pointer_cast<ColliderComponent>(
		m_Components[colliderComponentLocation]);
}

//Used to add components to the GameObject
void GameObject::AddComponent(std::shared_ptr<Component> component)
{
	m_Components.push_back(component);
	component->EnableComponent();


	if (component->GetType() == "graphics")
	{
		hasGraphicsComponent = true;
		graphicsComponentLocation = m_Components.size() - 1;
	}
	else if (component->GetType() == "transform")
	{
		hasTransformComponent = true;
		transformComponentLocation = m_Components.size() - 1;
	}
	else if (component->GetType() == "collider")
	{
		hasCollider = true;
		colliderComponentLocation = m_Components.size() - 1;
		this->GetColliderComponent()->Initialize(this->GetTransformComponent());
	}
}

//various setters & getters
void GameObject::SetActive()
{
	active = true;
}

void GameObject::SetInactive()
{
	active = false;
}

bool GameObject::IsActive()
{
	return active;
}
void GameObject::SetTag(std::string tag)
{
	tag = "" + tag;
}
std::string GameObject::GetTag()
{
	return tag;
}

void GameObject::SetUpdate()
{
	useUpdate = !useUpdate;
}

void GameObject::SetVelocity(float vel)
{
	velocity = vel;
}

bool GameObject::HasCollider()
{
	return hasCollider;
}
bool GameObject::HasUpdate()
{
	return useUpdate;
}

//Using this you can add GameObject as a child to the parent GameObject
void GameObject::AddChild(GameObject* childPtr)
{
	children.push_back(childPtr);
}

//Renders GameObject and its children to the scene
void GameObject::Render()
{
	
	const glm::mat4 zeroPointTransform = glm::mat4(1.0f);
	if (hasTransformComponent)
	{
		gameObjectTransform = this->GetTransformComponent()->GetTransform();
	} else
		gameObjectTransform = zeroPointTransform;

	if(hasGraphicsComponent)
	{
		this->GetGraphicsComponent()->Render(gameObjectTransform);
	}
	for (GameObject* child : children)
	{
		if (child->hasGraphicsComponent)
		{
			const auto absoluteTransform = gameObjectTransform * child->GetTransformComponent()->GetTransform();
			//child->GetGraphicsComponent()->Render(gameObjectTransform);
			child->GetGraphicsComponent()->Render(absoluteTransform);
		}
	}
}

//Sets move direction of GameObject
void GameObject::SetDirection(std::string dir)
{
	if (dir == "forward" || dir == "back" || dir == "right" || dir == "left" || dir == "up" || dir == "down")
		moveDirection = dir;
}

//Returns specific component from the GameObject's components list
std::shared_ptr<Component> GameObject::GetComponentByType(std::string type) 
{
	auto it = m_Components.begin();
	auto end = m_Components.end();
	for (it; it != end; ++it)
	{
		if ((*it)->GetType() == type)
		{
			return (*it);
		}
	}
#ifdef debuggingErrors 
	cout << "GameObject.cpp::getComponentByTypeAndSpecificType-" << "COMPONENT NOT FOUND ERROR!" << endl;
#endif
	return m_Components[0];
}

