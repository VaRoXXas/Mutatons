#include <pch.h>
#include "FrustumCulling/Vec3.h"
#include <math.h>
#include <stdio.h>


Vec3::Vec3(float x, float y, float z) {

	this->x = x;
	this->y = y;
	this->z = z;

}

Vec3::Vec3(const Vec3& v) {

	x = v.x;
	y = v.y;
	z = v.z;

}

Vec3::Vec3() {

	x = 0;
	y = 0;
	z = 0;

}

Vec3::~Vec3()
{

}

Vec3 Vec3::operator+(const Vec3& v) {//bez const

	Vec3 res;

	res.x = x + v.x;
	res.y = y + v.y;
	res.z = z + v.z;

	return(res);
}

Vec3 Vec3::operator-(const Vec3& v) {

	Vec3 res;

	res.x = x - v.x;
	res.y = y - v.y;
	res.z = z - v.z;

	return(res);
}

Vec3 Vec3::operator-(void) {

	Vec3 res;

	res.x = -x;
	res.y = -y;
	res.z = -z;

	return(res);
}

// cross product
Vec3 Vec3::operator*(Vec3& v) {

	Vec3 res;

	res.x = y * v.z - z * v.y;
	res.y = z * v.x - x * v.z;
	res.z = x * v.y - y * v.x;

	return (res);
}

Vec3 Vec3::operator*(float t) {

	Vec3 res;

	res.x = x * t;
	res.y = y * t;
	res.z = z * t;

	return (res);
}

Vec3 Vec3::operator/(float t) {

	Vec3 res;

	res.x = x / t;
	res.y = y / t;
	res.z = z / t;

	return (res);
}

float Vec3::Length() {

	return((float)sqrt(x * x + y * y + z * z));
}

void Vec3::Normalize() {

	float len;

	len = Length();
	if (len) {
		x /= len;;
		y /= len;
		z /= len;
	}
}


float Vec3::InnerProduct(Vec3& v) {

	return (x * v.x + y * v.y + z * v.z);
}

void Vec3::Copy(const Vec3& v) {

	x = v.x;
	y = v.y;
	z = v.z;
}

void Vec3::Set(float x, float y, float z) {

	this->x = x;
	this->y = y;
	this->z = z;
}

Vec3 Vec3::ScalarMult(float a) {

	Vec3 res;

	res.x = x * a;
	res.y = y * a;
	res.z = z * a;

	return res;
}

// -----------------------------------------------------------------------
// For debug pruposes: Prints a vector
// -----------------------------------------------------------------------

void Vec3::Print() {
	printf("Vec3(%f, %f, %f)", x, y, z);

}
