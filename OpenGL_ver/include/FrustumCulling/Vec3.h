#pragma once



class Vec3
{

public:

	float x, y, z;


	Vec3(float x, float y, float z);
	Vec3(const Vec3& v);
	Vec3();
	~Vec3();

	Vec3 operator +(const Vec3& v);
	Vec3 operator -(const Vec3& v);
	Vec3 operator *(Vec3& v);
	Vec3 operator *(float t);
	Vec3 operator /(float t);
	Vec3 operator -(void);

	float Length();
	void Normalize();
	float InnerProduct(Vec3& v);
	void Copy(const Vec3& v);
	void Set(float x, float y, float z);
	Vec3 ScalarMult(float a);

	void Print();

};
