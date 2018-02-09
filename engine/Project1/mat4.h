#pragma once

#include "vec3.h"

/*************************************************************************

Uusally when we put the elements of matrix, we think in a way of row matrix :
{ 1, 0, 0, transX, 0, 1, 0, transY, 0, 0, 1, transZ, 0, 0, 0, 1 };

Mat4 mat = { 1, 0, 0, transX,
0, 1, 0, transY,
0, 0, 1, transZ,
0, 0, 0, 1 };

In order for that to work, OpenGL expects the memory layout of T to be:

{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, transX, transY, transZ, 1 }

Mat4 T = { 1,      0,      0,      0,
0,      1,      0,      0,
0,      0,      1,      0,
transX, transY, transZ, 1 };

which is also called 'column major'.

## So Here is what I do. When you put all the elements in a way of row major matrix
which is : { 1, 0, 0, transX, 0, 1, 0, transY, 0, 0, 1, transZ, 0, 0, 0, 1 }

And transpose it in Set function, then it will saved as a way of column major.

##################################################
# Row-major vector

<memory layout - 1d array>
m0    m1    m2    m3
m4    m5    m6    m7
m8    m9    m10   m11
m12   m13   m14   m15

<memory layout - 2d array>
m00   m01   m02   m03
m10   m11   m12   m13
m20   m21   m22   m23
m30   m31   m32   m33

P|V = [ x  y  z ]

Left or Pre-multiplication : P|V * M

P' = P * T * Rz * Ry



                                 [1      0      0      0]

                                 [0      1      0      0]
[x' y' z' 1] = [x' y' z' 1]  *
                                 [0      0      1      0]

                                 [TransX TransY TransZ 1]


API : Direct X
##################################################
# Column-major vector

<memory layout - 1d array>
m0   m4   m8   m12
m1   m5   m9   m13
m2   m6   m10  m14
m3   m7   m11  m15

<memory layout - 2d array>
m00   m10   m20   m30
m01   m11   m21   m31
m02   m12   m22   m32
m03   m13   m23   m33

      [ x ]
P|V = [ y ]
      [ z ]

Right or Post-multiplication : M * P|V

P' = Ry * Rz * T * P


[ x' ]     [1  0  0  TransX]     [ x ]

[ y' ]     [0  1  0  TransY]     [ y ]
        =                     *
[ z' ]     [0  0  1  TransZ]     [ z ]

[ 1  ]     [0  0  0  1     ]     [ 1 ]

API: OpenGL
##########################################
(P, V, and M respectively stands for Point, Vector and Matrix).

*************************************************************************/

class Mat4
{
public:
	float m[16];

	static const Mat4 ZERO;
	static const Mat4 IDENTITY;
	//////////////////////////////////////////////////////////////////////////
	// Ctor & Dtor
	//////////////////////////////////////////////////////////////////////////

	/*************************************************************************
	Default Constructor.
	elements of matrix are initialized to I.

	1  0  0  0
	0  1  0  0
	0  0  1  0
	0  0  0  1

	*************************************************************************/
	Mat4();


	Mat4(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);

	/*************************************************************************
	column major order matrix.
	0   4   8   12
	1   5   9   13
	2   6   10  14
	3   7   11  15

	*************************************************************************/
	Mat4(const float* m);
	Mat4(const Mat4& copy);
	//////////////////////////////////////////////////////////////////////////
	// Public member function
	//////////////////////////////////////////////////////////////////////////

	void Set(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);
	void Set(const float* m);
	void Set(const Mat4& copy);
	void SetZero();
	void SetIdentity();

	bool IsZero() const;
	bool IsIdentity() const;

	bool Inverse();
	Mat4 GetInversed() const;

	void Transpose();
	Mat4 GetTransposed() const;

	/*
	<column-major matrix>
	m0   m4   m8   m12
	m1   m5   m9   m13
	m2   m6   m10  m14
	m3   m7   m11  m15

	1   0   0   x
	0   1   0   y
	0   0   1   z
	0   0   0   1
	*/
	void Translate(const Vec3& translation);
	void Translate(float xTranslation, float yTranslation, float zTranslation);
	/*
	<column-major matrix>
	m0   m4   m8   m12
	m1   m5   m9   m13
	m2   m6   m10  m14
	m3   m7   m11  m15

	x   0   0   0
	0   y   0   0
	0   0   z   0
	0   0   0   1
	*/
	void Scale(const Vec3& scale);
	void Scale(float scale);
	void Scale(float xScale, float yScale, float zScale);

	/*
	<column-major matrix>
	m0   m4   m8   m12
	m1   m5   m9   m13
	m2   m6   m10  m14
	m3   m7   m11  m15

	txx + c    txy + sz   txz - sy   0
	txy - sz   tyy + c    tyz + sx   0
	txz + sy   tyz - sx   tzz + c    0
	0          0          0          1
	*/
	//void Rotate(const Quaternion& q);
	void Rotate(const Vec3& axis, float angle);

	/*
	<column-major matrix>
	m0   m4   m8   m12
	m1   m5   m9   m13
	m2   m6   m10  m14
	m3   m7   m11  m15

	1   0     0    0
	0   cos  -sin  0
	0   sin   cos  0
	0   0     0    1
	*/

	void RotateX(float angle);
	/*
	<column-major matrix>
	m0   m4   m8   m12
	m1   m5   m9   m13
	m2   m6   m10  m14
	m3   m7   m11  m15

	cos   0   sin  0
	0     1   0    0
	-sin   0   cos  0
	0     0   0    1
	*/
	void RotateY(float angle);

	/*
	<column-major matrix>
	m0   m4   m8   m12
	m1   m5   m9   m13
	m2   m6   m10  m14
	m3   m7   m11  m15

	cos   -sin  0   0
	sin    cos  0   0
	0      0    1   0
	0      0    0   1
	*/
	void RotateZ(float angle);


	//////////////////////////////////////////////////////////////////////////
	// static member function
	//////////////////////////////////////////////////////////////////////////

	static Mat4 GetTranslation(const Vec3& translation);
	static Mat4 GetTranslation(float xTranslation, float yTranslation, float zTranslation);

	static Mat4 GetScaling(const Vec3& scale);
	static Mat4 GetScaling(float scale);
	static Mat4 GetScaling(float xScale, float yScale, float zScale);

	static Mat4 GetRotation(const Vec3& axis, float angle);
	static Mat4 GetRotationX(float angle);
	static Mat4 GetRotationY(float angle);
	static Mat4 GetRotationZ(float angle);

	//////////////////////////////////////////////////////////////////////////
	// Operator
	//////////////////////////////////////////////////////////////////////////
	Mat4 operator+(const Mat4& mat) const;
	Mat4& operator+=(const Mat4& mat);
	Mat4 operator-(const Mat4& mat) const;
	Mat4& operator-=(const Mat4& mat);
	Mat4 operator*(const Mat4& mat);
	Mat4& operator*=(const Mat4& mat);

	Mat4 operator*(float s) const;
	Mat4& operator*=(float s);
	Mat4 operator/ (float s) const;
	Mat4& operator/=(float s);

	Vec3 Mat4::operator*(const Vec3& v) const;

	bool operator==(const Mat4& mat) const;
	bool operator!=(const Mat4& mat) const;


	friend Vec3& operator*=(Vec3&v, const Mat4& mat);

private:
	// Help Function for Inverse
	float GetMinorMat(unsigned a0, unsigned a1, unsigned a2,
		unsigned a3, unsigned a4, unsigned a5,
		unsigned a6, unsigned a7, unsigned a8) const;
};
