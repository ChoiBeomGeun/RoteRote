#include "vector3.h"
#include <cmath>
Vector3::Vector3() : x(0.0f), y(0.0f), z(0.0f)
{
}

Vector3::Vector3(float tx, float ty, float tz)
	: x(tx), y(ty), z(tz)
{
}

void Vector3::Set(float tx, float ty, float tz)
{
	x = tx;
	y = ty;
	z = tz;
}

void Vector3::Set(const Vector3 & v)
{
	x = v.x;
	y = v.y;
	z = v.z;

}

void Vector3::SetZero()
{
	x = 0.f;
	y = 0.f;
	z = 0.f;
}

bool Vector3::IsZero() const
{
	return (x == 0.f) && (y == 0.f) && (z == 0.f);
}

bool Vector3::IsOne() const
{

	return (x == 1.f) && (y == 1.f) && (z == 1.f);
}

float Vector3::Distance(const Vector3 & v) const
{
	return sqrt(((x - v.x)* (x - v.x)) + ((y - v.y)*
		(y - v.y)) + ((z - v.z)* (z - v.z)));
}

float Vector3::DistanceSq(const Vector3 & v) const
{
	return ((x - v.x)* (x - v.x)) + ((y - v.y)*
		(y - v.y)) + ((z - v.z)* (z - v.z));
}

float Vector3::Length() const
{
	float l = sqrtf(x*x + y*y + z*z);
	return l;
}

float Vector3::Length(const Vector3 & v)
{
	float l = sqrtf(v.x * v.x + v.y * v.y + v.z* v.z);
	return l;
}

Vector3 & Vector3::Normalize()
{
	float l = Vector3::Length();

	x = x / l;
	y = y / l;
	z = z / l;

	return *this;
}

Vector3 Vector3::Normalize(Vector3& vec)
{
	float l = Vector3::Length(vec);

	Vector3 n;
	n.x = vec.x / l;
	n.y = vec.y / l;
	n.z = vec.z / l;

	return n;
}

float Vector3::DotProduct(const Vector3 & v) const
{
	return x*v.x + y*v.y + z*v.z;
}

float Vector3::DotProduct(const Vector3 & vec1, const Vector3 & vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

Vector3 Vector3::CrossProduct(const Vector3 & v) const
{
	Vector3 result;
	result.x = (y * v.z) - (z* v.y);
	result.y = (z * v.x) - (x* v.z);
	result.z = (x * v.y) - (y* v.x);

	return result;
}

Vector3 Vector3::CrossProduct(Vector3& v1, Vector3& v2)
{
	Vector3 result;
	result.x = (v1.y * v2.z) - (v1.z* v2.y);
	result.y = (v1.z * v2.x) - (v1.x* v2.z);
	result.z = (v1.x * v2.y) - (v1.y* v2.x);
	return result;
}


Vector3 Vector3::operator+(const Vector3 & v) const
{
	Vector3 result;
	result.x = this->x + v.x;
	result.y = this->y + v.y;
	result.z = this->z + v.z;

	return result;
}

Vector3& Vector3::operator+=(const Vector3 & v)
{
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;

	return *this;
}

Vector3 Vector3::operator-(const Vector3 & v) const
{
	Vector3 result;
	result.x = this->x - v.x;
	result.y = this->y - v.y;
	result.z = this->z - v.z;

	return result;

}

Vector3 Vector3::operator-=(const Vector3 & v)
{
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;

	return *this;
}

Vector3 Vector3::operator*(float s) const
{
	Vector3 result = *this;
	result.x *= s;
	result.y *= s;
	result.z *= s;

	return result;
}

Vector3 &Vector3::operator*=(float s)
{
	this->x *= s;
	this->y *= s;
	this->z *= s;

	return *this;
}

Vector3 Vector3::operator/(float s) const
{
	Vector3 result;
	result.x /= s;
	result.y /= s;
	result.z /= s;

	return result;
}

Vector3 Vector3::operator/=(float s)
{
	this->x /= s;
	this->y /= s;
	this->z /= s;

	return *this;
}

Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

bool Vector3::operator==(const Vector3 & v) const
{
	return (x == v.x) && (y == v.y) && (z == v.z);
}

bool Vector3::operator!=(const Vector3 & v) const
{
	return (x != v.x) || (y != v.y) || (z != v.z);
}

Vector3 operator*(float scale, const Vector3 & rhs)
{
	return Vector3(scale * rhs.x, scale * rhs.y, scale * rhs.z);
}
