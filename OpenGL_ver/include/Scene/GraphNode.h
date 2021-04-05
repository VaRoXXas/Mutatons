#pragma once

class Shader;
class PseudoMesh;
class Model;



class GraphNode
{
public:
	GraphNode(PseudoMesh* pseudoMesh, glm::mat4 localTransform);
	GraphNode(Model* model, glm::mat4 localTransform);
	GraphNode(glm::mat4 localTransform);

	void Render(glm::mat4 parentTransform = glm::mat4(1.0f));
	void AddChild(GraphNode* child);
	//void AddChild(GraphNode& child);

private:
	PseudoMesh* pseudoMesh;
	Model* model;
	glm::mat4 localTransform;

	std::vector<GraphNode*> children;
};
