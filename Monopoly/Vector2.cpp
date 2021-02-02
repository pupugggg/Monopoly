#include"Vector2.h"
Vector2::Vector2() :x(0), y(0) {}
Vector2::Vector2(int inX, int inY) : x(inX), y(inY) {}
void Vector2::operator=(const Vector2& v)
{
	x = v.x;
	y = v.y;
}