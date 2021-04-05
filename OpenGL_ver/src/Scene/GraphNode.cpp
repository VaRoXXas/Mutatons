#include "pch.h"
#include "Scene/GraphNode.h"
#include "Rendering/PseudoMesh.h"
#include "Rendering/Model.h"

extern Shader* litTexturedShaderPtr;
extern Shader* litTexturedInstancedShaderPtr;
extern Shader* orbitShaderPtr;
extern Shader* sphereShaderPtr;



GraphNode::GraphNode(PseudoMesh* pseudoMesh, glm::mat4 localTransform) : pseudoMesh(pseudoMesh), localTransform(localTransform)
{}

GraphNode::GraphNode(Model* model, glm::mat4 localTransform) : model(model), localTransform(localTransform)
{}

GraphNode::GraphNode(glm::mat4 localTransform) : localTransform(localTransform)
{}

void GraphNode::Render(glm::mat4 parentTransform)
{
	glm::mat4 absoluteTransform = parentTransform * localTransform;

	if (pseudoMesh)
		pseudoMesh->Render(absoluteTransform); // rendering mesh in its actual position
	if (model)
		model->CustomRender(*litTexturedShaderPtr, absoluteTransform);

	for(GraphNode* child : children)
	{
		child->Render(absoluteTransform); // recursion -> this absolute transform is child's parent transform
	}
}

void GraphNode::AddChild(GraphNode* child)
{
	children.push_back(child);
}

//void GraphNode::AddChild(GraphNode& child)
//{
//	children.push_back(&child);
//}
