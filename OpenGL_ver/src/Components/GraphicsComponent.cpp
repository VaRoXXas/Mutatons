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
	return type;
}

void GraphicsComponent::DisableComponent() {
	enabled = false;
}

void GraphicsComponent::EnableComponent() {
	enabled = true;
}

bool GraphicsComponent::Enabled() {
	return enabled;
}
