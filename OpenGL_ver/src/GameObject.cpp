#include "pch.h"
#include "GameObject.h"
#include "Component.h"
#include "Components/GraphicsComponent.h"
#include "Components/TransformComponent.h"
#include "Components/UpdateComponent.h"

//#include "GameObjectSharer.h"
//#include "DevelopState.h"
//#include "RectColliderComponent.h"



void GameObject::Update(glm::vec3& locationVec)
{
	//std::cout << useUpdate << std::endl;
	if (active && useUpdate)
	{
		this->GetTransformComponent()->SetLocation(locationVec);
		if (moveDirection == "forward")
		{
			locationVec.z = locationVec.z + 0.0001f;
		}
		else if (moveDirection == "back")
		{
			locationVec.z = locationVec.z - 0.0001f;
		}
		else if (moveDirection == "right")
		{
			locationVec.x = locationVec.x - 0.0001f;
		}
		else if (moveDirection == "left")
		{
			locationVec.x = locationVec.x + 0.0001f;
		}
	}
}

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
		numberRectColliderComponents++;
		if (numberRectColliderComponents == 1)
		{
			firstRectColliderComponentLocation =
				m_Components.size() - 1;
		}
	}
}

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
	m_Tag = "" + tag;
}
std::string GameObject::GetTag()
{
	return m_Tag;
}

void GameObject::AddChild(GameObject* childPtr)
{
	children.push_back(childPtr);
}

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

		for(GameObject* child : children)
		{
			if (child->hasGraphicsComponent)
			{
				child->GetGraphicsComponent()->Render(gameObjectTransform);
			}
		}
	}
}

void GameObject::SetDirection(std::string dir)
{
	if (dir == "forward" || dir == "back" || dir == "right" || dir == "left")
		moveDirection = dir;
}

void GameObject::SetUpdate()
{
	useUpdate = !useUpdate;
}

/*void GameObject::start(GameObjectSharer* gos)
{
	auto it = m_Components.begin();
	auto end = m_Components.end();
	for (it;
		it != end;
		++it)
	{
		(*it)->start(gos, this);
	}
}*/

// Slow - only use in start function
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

/*
FloatRect& GameObject::GetEncompassingRectCollider()
{
 if (m_HasCollider)
 {
 return (static_pointer_cast<RectColliderComponent>(
 m_Components[m_FirstRectColliderComponentLocation]))
 ->getColliderRectF();
 }
}
string GameObject::GetEncompassingRectColliderTag()
{
 return static_pointer_cast<RectColliderComponent>(
 m_Components[m_FirstRectColliderComponentLocation])->
 getColliderTag();
}
*/

bool GameObject::HasCollider()
{
	return hasCollider;
}
bool GameObject::HasUpdate()
{
	return useUpdate;
}
