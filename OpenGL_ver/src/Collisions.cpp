#include "pch.h"
#include "Collisions.h"

float Collisions::Clamp(float x, float min, float max)
{
    if (x <= min)
    return min;
    if (min < x && x < max)
        return x;
    if (max <= x)
    return max;
}

bool Collisions::RectCol()
{

}

bool Collisions::Trigger()
{

}