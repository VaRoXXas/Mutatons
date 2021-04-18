#include "pch.h"
#include "Component.h"
#include "Components/GraphicsComponent.h"
#include "Components/TransformComponent.h"
#include "Rendering/Shader.h"
#include "Rendering/deprecated/Model.h"

//#include "GameObjectSharer.h"
extern Shader* litTexturedShaderPtr;
extern Shader* litTexturedInstancedShaderPtr;
extern Shader* orbitShaderPtr;
extern Shader* sphereShaderPtr;



GraphicsComponent::GraphicsComponent()
{

}

void GraphicsComponent::SetModel(Model* modelLoad)
{
	modelPtr = modelLoad;
}

//void GraphicsComponent::Draw(std::shared_ptr<TransformComponent> t)
//{
//	modelPtr->CustomRender(*litTexturedShaderPtr, t->GetLocation());
//}

void GraphicsComponent::Draw(Shader& shader, const glm::mat4& transform)
{
	modelPtr->CustomRender(shader, transform);
}

void GraphicsComponent::Render(const glm::mat4& transform)
{
	if (modelPtr)
		modelPtr->CustomRender(*litTexturedShaderPtr, transform);
}

std::string GraphicsComponent::GetType() {
	return m_Type;
}

void GraphicsComponent::DisableComponent() {
	m_Enabled = false;
}

void GraphicsComponent::EnableComponent() {
	m_Enabled = true;
}

bool GraphicsComponent::Enabled() {
	return m_Enabled;
}
