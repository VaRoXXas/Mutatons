#pragma once
class Collisions
{
public:
	float Clamp(float x, float min, float max);
	bool RectCol();
	bool Trigger();
};

