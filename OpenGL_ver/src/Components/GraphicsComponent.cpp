#include "pch.h"
#include "Component.h"
#include "Rendering/animation/Animation.h"
#include "Rendering/animation/Animator.h"
#include "Components/GraphicsComponent.h"
#include "Components/TransformComponent.h"
#include "Rendering/Shader.h"
#include "Rendering/Model.h"
#include "GameObject/GameObject.h"

//#include "GameObjectSharer.h"
extern Shader* litTexturedShaderPtr;
extern Shader* litTexturedInstancedShaderPtr;
extern Shader* simpleDepthShaderPtr;
extern Shader* orbitShaderPtr;
extern Shader* sphereShaderPtr;
extern Shader* unlitTexturedAnimatedShaderPtr;



//assigns model to the gameobject
void GraphicsComponent::SetModel(Model* modelLoad)
{
	modelPtr = modelLoad;
}

void GraphicsComponent::SetHighlighted(bool value)
{
	isHighlighted = value;
}

//renders assigned model to the scene
void GraphicsComponent::Render(const glm::mat4& transform)
{
	if (modelPtr && !animated)
	{
		if (isHighlighted)
		{
			litTexturedShaderPtr->Use();
			litTexturedShaderPtr->SetBool("isHighlighted", true);
		}
		else
		{
			litTexturedShaderPtr->Use();
			litTexturedShaderPtr->SetBool("isHighlighted", false);
		}
		
		modelPtr->CustomRender(*litTexturedShaderPtr, transform);
	}
	else if (modelPtr && animated)
	{
		modelPtr->CustomRender(*unlitTexturedAnimatedShaderPtr, transform);
	}
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

//sets if object will be animated
void GraphicsComponent::InitializeAnimation(const std::string &path)
{
	animated = true;
	animPtr = new Animation(path, modelPtr);
	animatorPtr = new Animator(animPtr);
}

Animator* GraphicsComponent::GetAnimator()
{
	return animatorPtr;
}


bool GraphicsComponent::GetAnimated()
{
	return animated;
}