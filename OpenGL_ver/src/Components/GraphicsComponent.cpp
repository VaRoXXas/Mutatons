#include "pch.h"
#include "Component.h"
#include "Components/GraphicsComponent.h"
#include "Components/TransformComponent.h"
#include "Rendering/Shader.h"
#include "Rendering/deprecated/Model.h"

//#include "GameObjectSharer.h"
extern Shader* litTexturedShaderPtr;
extern Shader* litTexturedInstancedShaderPtr;
extern Shader* simpleDepthShaderPtr;
extern Shader* orbitShaderPtr;
extern Shader* sphereShaderPtr;



//assigns model to the gameobject
void GraphicsComponent::SetModel(Model* modelLoad)
{
	modelPtr = modelLoad;
}

//renders assigned model to the scene
void GraphicsComponent::Render(const glm::mat4& transform)
{
	if (modelPtr)
		modelPtr->CustomRender(*litTexturedShaderPtr, transform);
}

void GraphicsComponent::DepthRender(const glm::mat4& transform)
{
	if (modelPtr)
		modelPtr->CustomRender(*simpleDepthShaderPtr, transform);
}

//returns component's type
std::string GraphicsComponent::GetType() {
	return type;
}

//functions used to disable and enable component
void GraphicsComponent::DisableComponent() {
	enabled = false;
}

void GraphicsComponent::EnableComponent() {
	enabled = true;
}

//returns if component is enabled
bool GraphicsComponent::Enabled() {
	return enabled;
}
