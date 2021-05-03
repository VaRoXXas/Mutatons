#include <pch.h>
#include "FrustumCulling/Plane.h"
#include <stdio.h>



Plane::Plane(Vec3& v1, Vec3& v2, Vec3& v3) {

	Set3Points(v1, v2, v3);
}

Plane::Plane() {}

Plane::~Plane() {}

void Plane::Set3Points(Vec3& v1, Vec3& v2, Vec3& v3) {


	Vec3 aux1, aux2;

	aux1 = v1 - v2;
	aux2 = v3 - v2;

	normal = aux2 * aux1;

	normal.Normalize();
	point.Copy(v2);
	d = -(normal.InnerProduct(point));
}

void Plane::SetNormalAndPoint(Vec3& normal, Vec3& point) {

	this->normal.Copy(normal);
	this->normal.Normalize();
	d = -(this->normal.InnerProduct(point));
}

void Plane::SetCoefficients(float a, float b, float c, float d) {

	// set the normal vector
	normal.Set(a, b, c);
	//compute the lenght of the vector
	float l = normal.Length();
	// normalize the vector
	normal.Set(a / l, b / l, c / l);
	// and divide d by th length as well
	this->d = d / l;
}

float Plane::Distance(Vec3& p) {

	return (d + normal.InnerProduct(p));
}

void Plane::Print() {

	printf("Plane("); normal.Print(); printf("# %f)", d);
}
