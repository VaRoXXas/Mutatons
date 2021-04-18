#include "pch.h"
#include "Component.h"
#include "Components/UpdateComponent.h"
#include "Components/TransformComponent.h"



void UpdateComponent::Update(float fps, glm::mat4 *transform)
{
	if (objectBehavior == 1)
		*transform = glm::translate(*transform, glm::vec3(0.0f, 0.0001f, 0.0f)*fps);
}
void UpdateComponent::SetBehavior(std::string chooseOne)
{
	if (chooseOne == "mutaton")
	{
		objectBehavior = 1;
	}
}
std::string UpdateComponent::GetType() {
	return m_Type;
}
void UpdateComponent::DisableComponent() {
	m_Enabled = false;
}
void UpdateComponent::EnableComponent() {
	m_Enabled = true;
}
bool UpdateComponent::Enabled() {
	return m_Enabled;
}
