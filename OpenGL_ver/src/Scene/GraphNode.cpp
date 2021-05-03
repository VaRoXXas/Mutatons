// ReSharper disable CppClangTidyClangDiagnosticShadowFieldInConstructor
// ReSharper disable CppUseAuto
#include "pch.h"
#include "Components/GraphicsComponent.h"
#include "GameObject/GameObject.h"
#include "Scene/GraphNode.h"
#include "Rendering/PseudoMesh.h"
#include "Rendering/Model.h"



extern Shader* litTexturedShaderPtr;
extern Shader* litTexturedInstancedShaderPtr;
extern Shader* orbitShaderPtr;
extern Shader* sphereShaderPtr;



GraphNode::GraphNode(PseudoMesh* pseudoMesh, const glm::mat4& localTransform) : transform(localTransform), pseudoMeshPtr(pseudoMesh)
{
	
}

GraphNode::GraphNode(Model* model, const glm::mat4& localTransform) : transform(localTransform), modelPtr(model)
{
	
}

GraphNode::GraphNode(GameObject* gameObject, const glm::mat4& localTransform) : transform(localTransform), gameObjectPtr(gameObject)
{

}

GraphNode::GraphNode(const glm::mat4& localTransform) : transform(localTransform)
{
	
}

void GraphNode::Render(const glm::mat4& parentTransform) const
{
	const auto absoluteTransform = parentTransform * transform;

	if (pseudoMeshPtr)
		pseudoMeshPtr->Draw(absoluteTransform); // rendering mesh in its current position
	else if (modelPtr)
		modelPtr->CustomRender(*litTexturedShaderPtr, absoluteTransform);
	else if (gameObjectPtr)
	{
		std::shared_ptr<GraphicsComponent> gcPtr = gameObjectPtr->GetGraphicsComponent();
		//std::shared_ptr<TransformComponent> tPtr = gameObjectPtr->GetTransformComponent();
		gcPtr->Render(absoluteTransform);
	}

	for(const GraphNode* child : children)
	{
		child->Render(absoluteTransform); // recursion -> this absolute transform is child's parent transform
	}
}

void GraphNode::AddChild(const GraphNode* childPtr)
{
	children.push_back(childPtr);
}

/*
void GraphNode::AddChild(const GraphNode& child)
{
	children.push_back(child);
}
*/
