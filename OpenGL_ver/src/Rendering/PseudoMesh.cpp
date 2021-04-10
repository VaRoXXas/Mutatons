// ReSharper disable CppClangTidyClangDiagnosticShadowFieldInConstructor
#include "pch.h"
#include "Rendering/PseudoMesh.h"



void PseudoMesh::Draw(const glm::mat4& transformMatrix) const
{
	if (drawFunctionWithSizeAndColorPtr != nullptr)
		drawFunctionWithSizeAndColorPtr(size, colorPtr, transformMatrix);
	else if (drawFunctionPtr != nullptr)
		drawFunctionPtr(transformMatrix);
}

PseudoMesh::PseudoMesh(void (*drawFunctionPtr)(const float&, const float*, const glm::mat4&), const float& sizeArg, const float* colorArgPtr) : drawFunctionWithSizeAndColorPtr(drawFunctionPtr), size(sizeArg)
{
	for (unsigned short i = 0; i < 4; i++)
	{
		if (colorArgPtr == nullptr)
		{
			std::cerr << "Error::PseudoMesh::Constructor::EmptyColor\n";
		}
		else
		{
			const auto copy = colorArgPtr[i];
			colorPtr[i] = copy;
		}
	}
}

PseudoMesh::PseudoMesh(void(* drawFunctionPtr)(const glm::mat4&))
: drawFunctionPtr(drawFunctionPtr)
{
	
}

PseudoMesh::~PseudoMesh()
{
	delete[] colorPtr;
}
