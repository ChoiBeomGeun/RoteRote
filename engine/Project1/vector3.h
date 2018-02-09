#pragma once

class Vector3
{
public:
	float x;
	float y;
	float z;

	Vector3();

	Vector3(float x, float y, float z);
	void Set(float x, float y, float z);
	void Set(const Vector3 & v);

	void SetZero();

	bool IsZero() const;
	bool IsOne() const;

	float Distance(const Vector3 & v) const;
	float DistanceSq(const Vector3 & v) const;

	float Length() const;
	static float Length(const Vector3& v);

	Vector3& Normalize();
	static Vector3 Normalize(Vector3& );
	float DotProduct(const Vector3 & v) const;
	static float  DotProduct(const Vector3 &vec1, const Vector3 &vec2);
	Vector3 CrossProduct(const Vector3 & v) const;
	static Vector3 CrossProduct(Vector3& v1, Vector3& v2);
	Vector3 operator+(const Vector3 & v) const;
	Vector3 &operator+=(const Vector3 & v);
	Vector3 operator-(const Vector3 & v) const;
	Vector3 operator-=(const Vector3 & v);

	Vector3 operator*(float s) const;
	Vector3 &operator*=(float s);

	friend	Vector3 operator*(float scale, const Vector3 &rhs);

	Vector3 operator/(float s) const;
	Vector3 operator/=(float s);

	Vector3 operator-() const;

	bool operator == (const Vector3 & v)const;
	bool operator != (const Vector3 & v)const;

};