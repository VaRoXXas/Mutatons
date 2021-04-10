#pragma once



// This class is designed to handle meshes generated on the fly using geometry shaders and other non-standard meshes, which have their own drawing function defined in code.
class PseudoMesh
{
public:

	// Calls the drawing function set for this PseudoMesh.
	void Draw(const glm::mat4& transformMatrix) const;
	
	PseudoMesh(void (*drawFunctionPtr)(const float&, const float*, const glm::mat4&), const float& sizeArg, const float* colorArgPtr);
	PseudoMesh(void (*drawFunctionPtr)(const glm::mat4&));
	~PseudoMesh();
	
private:

	// Drawing functions' pointers.
	void (*drawFunctionWithSizeAndColorPtr)(const float&, const float*, const glm::mat4&) = nullptr;
	void (*drawFunctionPtr)(const glm::mat4&) = nullptr;

	// For objects being drawn using geometry shader.
	float size = 0;
	float* colorPtr = new float[4];
};
