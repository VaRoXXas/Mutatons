#include "pch.h"
#include "Component.h"
#include "Components/GraphicsComponent.h"
#include "Components/TransformComponent.h"
#include "GameObject.h"
//#include "GameObjectSharer.h"
//#include "DevelopState.h"
//#include "RectColliderComponent.h"



void GameObject::Update(float fps)
{
	if (m_Active && m_HasUpdateComponent)
	{
		for (int i = m_FirstUpdateComponentLocation; i < m_FirstUpdateComponentLocation + m_NumberUpdateComponents; i++)
		{
			std::shared_ptr<UpdateComponent> tempUpdate = std::static_pointer_cast<UpdateComponent>(m_Components[i]);
			if (tempUpdate->Enabled())
			{
				tempUpdate->Update(fps);
			}
		}
	}
}

/*void GameObject::Draw(GLFWwindow& window)
{
	if (m_Active && m_HasGraphicsComponent)
	{
		if (m_Components[m_GraphicsComponentLocation]->Enabled())
		{
			GetGraphicsComponent()->Draw(GetTransformComponent());
		}
	}
}*/

std::shared_ptr<GraphicsComponent> GameObject::GetGraphicsComponent()
{
	return std::static_pointer_cast<GraphicsComponent>(
		m_Components[m_GraphicsComponentLocation]);
}

std::shared_ptr<TransformComponent> GameObject::GetTransformComponent()
{
	return std::static_pointer_cast<TransformComponent>(
		m_Components[m_TransformComponentLocation]);
}

void GameObject::AddComponent(std::shared_ptr<Component> component)
{
	m_Components.push_back(component);
	component->EnableComponent();

	if (component->GetType() == "update")
	{
		m_HasUpdateComponent = true;
		m_NumberUpdateComponents++;
		if (m_NumberUpdateComponents == 1)
		{
			m_FirstUpdateComponentLocation =
				m_Components.size() - 1;
		}
	}
	else if (component->GetType() == "graphics")
	{
		m_HasGraphicsComponent = true;
		m_GraphicsComponentLocation = m_Components.size() - 1;
	}
	else if (component->GetType() == "transform")
	{
		m_HasTransformComponent = true;
		m_TransformComponentLocation = m_Components.size() - 1;
	}
	else if (component->GetType() == "collider")
	{
		m_HasCollider = true;
		m_NumberRectColliderComponents++;
		if (m_NumberRectColliderComponents == 1)
		{
			m_FirstRectColliderComponentLocation =
				m_Components.size() - 1;
		}
	}
}

void GameObject::SetActive()
{
	m_Active = true;
}

void GameObject::SetInactive()
{
	m_Active = false;
}

bool GameObject::IsActive()
{
	return m_Active;
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
	if (m_HasTransformComponent)
	{
		gameObjectTransform = this->GetTransformComponent()->GetTransform();
	} else
		gameObjectTransform = zeroPointTransform;

	if(m_HasGraphicsComponent)
	{
		this->GetGraphicsComponent()->Render(gameObjectTransform);

		for(GameObject* child : children)
		{
			if (child->m_HasGraphicsComponent)
			{
				child->GetGraphicsComponent()->Render(gameObjectTransform);
			}
		}
	}
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

std::shared_ptr<UpdateComponent> GameObject::GetFirstUpdateComponent()
{
	return std::static_pointer_cast<UpdateComponent>(
		m_Components[m_FirstUpdateComponentLocation]);
}

bool GameObject::HasCollider()
{
	return m_HasCollider;
}
bool GameObject::HasUpdateComponent()
{
	return m_HasUpdateComponent;
}