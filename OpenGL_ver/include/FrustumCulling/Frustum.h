#pragma once
//View Frustum

#include "FrustumCulling/Vec3.h"

class Vec3;

#include "FrustumCulling/Plane.h"

class Plane;

#include "FrustumCulling/AABox.h"

class AABox;



class Frustum
{
private:

	enum {
		TOP = 0,
		BOTTOM,
		LEFT,
		RIGHT,
		NEARP,
		FARP
	};


public:

	static enum { OUTSIDE, INTERSECT, INSIDE };

	Plane pl[6];


	Vec3 ntl, ntr, nbl, nbr, ftl, ftr, fbl, fbr;
	float nearD, farD, ratio, angle, tang;
	float nw, nh, fw, fh;

	Frustum();
	~Frustum();

	void SetCamInternals(float angle, float ratio, float nearD, float farD);
	void SetCamDef(Vec3& p, Vec3& l, Vec3& u);
	int PointInFrustum(Vec3& p);
	int SphereInFrustum(Vec3& p, float raio);
	int BoxInFrustum(AABox& b);
};
