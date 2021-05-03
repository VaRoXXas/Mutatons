//Axis Aligned Boxes
#include "pch.h"
#include "FrustumCulling/AABox.h"
#include "FrustumCulling/Vec3.h"




AABox::AABox(Vec3& corner, float x, float y, float z) {

	SetBox(corner, x, y, z);
}

AABox::AABox(void) {


	corner.x = 0; corner.y = 0; corner.z = 0;

	x = 1.0f;
	y = 1.0f;
	z = 1.0f;

}

AABox::~AABox() {}

void AABox::SetBox(Vec3& corner, float x, float y, float z) {


	this->corner.Copy(corner);

	if (x < 0.0) {
		x = -x;
		this->corner.x -= x;
	}
	if (y < 0.0) {
		y = -y;
		this->corner.y -= y;
	}
	if (z < 0.0) {
		z = -z;
		this->corner.z -= z;
	}
	this->x = x;
	this->y = y;
	this->z = z;


}

Vec3 AABox::GetVertexP(Vec3& normal) {

	Vec3 res = corner;

	if (normal.x > 0)
		res.x += x;

	if (normal.y > 0)
		res.y += y;

	if (normal.z > 0)
		res.z += z;

	return(res);
}

Vec3 AABox::GetVertexN(Vec3& normal) {

	Vec3 res = corner;

	if (normal.x < 0)
		res.x += x;

	if (normal.y < 0)
		res.y += y;

	if (normal.z < 0)
		res.z += z;

	return(res);
}
