#include "pch.h"
#include "Component.h"
#include "Components/GraphicsComponent.h"
#include "Components/TransformComponent.h"
#include "Rendering/Shader.h"


//#include "GameObjectSharer.h"


GraphicsComponent::GraphicsComponent()
{
}

void GraphicsComponent::LoadModel(Model* modelLoad)
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

