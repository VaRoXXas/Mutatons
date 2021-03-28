#pragma once
#include "Shader.h"

extern Shader* orbitShaderPtr;
extern Shader* sphereShaderPtr;

class PseudoMesh
{
public:
	void Render(glm::mat4 transform) const
	{
		if (drawFunctionG)
			drawFunctionG(size, mColor, transform);
		else if (drawFunction)
			drawFunction(transform);
	}

	PseudoMesh(void (*drawFunctionArg)(const float, float*, glm::mat4), float size, float* color) : drawFunctionG(drawFunctionArg), size(size)
	{
		for (int i = 0; i < 4; i++)
		{
			if (!color)
				std::cerr << "PUSTY KOLOR!!!";
			float copy = color[i];
			mColor[i] = copy;
		}
		drawFunction = nullptr;
	}

	PseudoMesh(void (*drawFunctionArg)(glm::mat4)) : drawFunction(drawFunctionArg)
	{
		size = 0;
		drawFunctionG = nullptr;
	}

	~PseudoMesh()
	{
		delete[] mColor;
	}
	
private:
	void (*drawFunctionG)(const float, float*, glm::mat4); // function pointer to draw this PseudoMesh
	void (*drawFunction)(glm::mat4);
	float size;
	float* mColor = new float[4];
};
