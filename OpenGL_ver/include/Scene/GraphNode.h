#pragma once

class PseudoMesh;
class Model;
class GameObject;


class GraphNode
{
public:
	
	GraphNode(PseudoMesh* pseudoMesh, const glm::mat4& localTransform);
	GraphNode(GameObject* gameObject, const glm::mat4& localTransform);
	GraphNode(Model* model, const glm::mat4& localTransform);
	GraphNode(const glm::mat4& localTransform);

	// Draws the associated objects and all children's associated objects.
	void Render(const glm::mat4& parentTransform = glm::mat4(1.0f)) const;

	// Assigns a new child to this GraphNode.
	void AddChild(const GraphNode* childPtr);
	//void AddChild(const GraphNode& child);

private:
	
	glm::mat4 transform;
	PseudoMesh* pseudoMeshPtr = nullptr;
	Model* modelPtr = nullptr;
	GameObject* gameObjectPtr = nullptr;
	
	std::vector<const GraphNode*> children;
};
