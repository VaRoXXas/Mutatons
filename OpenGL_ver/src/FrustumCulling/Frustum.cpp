//View Frustum
#include <pch.h>
#include "FrustumCulling/Frustum.h"
#include <math.h>

#define ANG2RAD 3.14159265358979323846/180.0



Frustum::Frustum() {}

Frustum::~Frustum() {}

void Frustum::SetCamInternals(float angle, float ratio, float nearD, float farD) {

	this->ratio = ratio;
	this->angle = angle;
	this->nearD = nearD;
	this->farD = farD;

	tang = (float)tan(angle * ANG2RAD * 0.5);
	nh = nearD * tang;
	nw = nh * ratio;
	fh = farD * tang;
	fw = fh * ratio;


}

void Frustum::SetCamDef(Vec3& p, Vec3& l, Vec3& u) {

	Vec3 dir, nc, fc, X, Y, Z;

	Z = p - l;
	Z.Normalize();

	X = u * Z;
	X.Normalize();

	Y = Z * X;

	nc = p - Z * nearD;
	fc = p - Z * farD;

	ntl = nc + Y * nh - X * nw;
	ntr = nc + Y * nh + X * nw;
	nbl = nc - Y * nh - X * nw;
	nbr = nc - Y * nh + X * nw;

	ftl = fc + Y * fh - X * fw;
	ftr = fc + Y * fh + X * fw;
	fbl = fc - Y * fh - X * fw;
	fbr = fc - Y * fh + X * fw;

	pl[TOP].Set3Points(ntr, ntl, ftl);
	pl[BOTTOM].Set3Points(nbl, nbr, fbr);
	pl[LEFT].Set3Points(ntl, nbl, fbl);
	pl[RIGHT].Set3Points(nbr, ntr, fbr);
	pl[NEARP].Set3Points(ntl, ntr, nbr);
	pl[FARP].Set3Points(ftr, ftl, fbl);
}

int Frustum::PointInFrustum(Vec3& p) {

	int result = INSIDE;
	for (int i = 0; i < 6; i++) {

		if (pl[i].Distance(p) < 0)
			return OUTSIDE;
	}
	return(result);

}

int Frustum::SphereInFrustum(Vec3& p, float raio) {

	int result = INSIDE;
	float distance;

	for (int i = 0; i < 6; i++) {
		distance = pl[i].Distance(p);
		if (distance < -raio)
			return OUTSIDE;
		else if (distance < raio)
			result = INTERSECT;
	}
	return(result);

}

/*int Frustum::boxInFrustum(AABox& b) {

	int result = INSIDE;
	for (int i = 0; i < 6; i++) {

		if (pl[i].distance(b.GetVertexP(pl[i].normal)) < 0)
			return OUTSIDE;
		else if (pl[i].distance(b.GetVertexN(pl[i].normal)) < 0)
			result = INTERSECT;
	}
	return(result);

}*/
