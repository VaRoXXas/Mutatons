#pragma once
//Axis Aligned Boxes

#include "FrustumCulling/Vec3.h"

class Vec3;



class AABox
{

public:

	Vec3 corner;
	float x, y, z;


	AABox(Vec3& corner, float x, float y, float z);
	AABox(void);
	~AABox();

	void SetBox(Vec3& corner, float x, float y, float z);

	// for use in frustum computations
	Vec3 GetVertexP(Vec3& normal);
	Vec3 GetVertexN(Vec3& normal);


};
