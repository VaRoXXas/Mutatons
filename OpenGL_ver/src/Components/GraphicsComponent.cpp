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

void GraphicsComponent::Render(const glm::mat4& transform)
{
	const auto absoluteTransform = transform;

	if (modelPtr)
		modelPtr->CustomRender(*litTexturedShaderPtr, absoluteTransform);
}
