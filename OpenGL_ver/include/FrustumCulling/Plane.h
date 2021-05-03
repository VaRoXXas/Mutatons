#pragma once

#include "FrustumCulling/Vec3.h"

class Vec3;



class Plane
{

public:

	Vec3 normal, point;
	float d;


	Plane(Vec3& v1, Vec3& v2, Vec3& v3);
	Plane(void);
	~Plane();

	void Set3Points(Vec3& v1, Vec3& v2, Vec3& v3);
	void SetNormalAndPoint(Vec3& normal, Vec3& point);
	void SetCoefficients(float a, float b, float c, float d);
	float Distance(Vec3& p);

	void Print();

};
